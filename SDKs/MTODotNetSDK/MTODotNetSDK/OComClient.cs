using MTODotNetSDK.DTOs;
using MTODotNetSDK.Exceptions;
using MTODotNetSDK.Interfaces;
using System.Drawing;
using System.Runtime.Serialization;
using System.Text;
using System.Threading;
using MTODotNetSDK.Extensions;

namespace MTODotNetSDK
{
    public class OComClient : IOCom
    {
        public static readonly VersionT Version = new VersionT(0, 1, 0, 0);
        private readonly IComChannel channel;
        private readonly Encoding encoding = Encoding.ASCII;

        public bool IsConnected { get; private set; }

        private UInt16 sessionId = 0;

        public OComClient(IComChannel channel)
        {
            this.channel = channel;
        }

        public event Action<MsgHeader> OnRecived;

        public async Task<SystemInfo> Connect()
        {
            sessionId = 0;
            SystemInfo info = new SystemInfo();

            await Send(new MsgHeader(MsgType.MT_HS_INIT, 0, 4),
                async () =>
            {
                await channel.Write(Version);
            },
                async response =>
            {
                if (response.Type == MsgType.MT_HS_ACK)
                {
                    sessionId = response.SessionId;
                    info = await channel.Read<SystemInfo>();

                    IsConnected = true;
                }
                else if (response.Type == MsgType.MT_HS_NAK)
                    throw new HSFailedException((await channel.Read<VersionT>()));
            });

            return info;
        }

        public async Task Disconnect()
        {
            if (!IsConnected)
                return;

            await Send(new MsgHeader(MsgType.MT_DISCONNECT, sessionId, 0));

            IsConnected = false;
        }

        public async Task<MsgHeader> Send(MsgHeader message, Func<Task>? payloadBuilder = null, Func<MsgHeader, Task>? responseHandler = null)
        {
            await channel.Write(message);

            if (payloadBuilder != null)
                await payloadBuilder();

            MsgHeader response = await channel.Read<MsgHeader>();

            switch (response.Type)
            {
                case MsgType.MT_TIMEOUT:
                    throw new TimeoutException("Timed out");
                case MsgType.MT_DISCONNECT:
                    throw new Exception("Disconnected");
                case MsgType.MS_NOT_AVAILABLE:
                    throw new Exception("Session not available");
                case MsgType.MT_UNKNOWN:
                    throw new Exception($"{message.Type} is a unknown command");
                case MsgType.MT_ERROR:
                    throw new Exception("A Error occured");
                default:
                    break;
            }

            if(responseHandler != null)
            {
                await responseHandler(response);
            }

            return response;
        }

        public void Dispose()
        {
            Disconnect().Wait();
            channel.Dispose();
        }

        public async ValueTask DisposeAsync()
        {
            await Disconnect();
            await channel.DisposeAsync();
        }

        public async Task<DateTime> GetTime()
        {
            DateTime dateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);

            await Send(new MsgHeader(MsgType.MT_TIME_GET, sessionId, 0), null, 
                async response => 
            {
                if (response.Type == MsgType.MT_TIME_INFO && response.Size == sizeof(UInt32))
                {
                    dateTime = dateTime.AddSeconds(await channel.Read<UInt32>()).ToLocalTime();
                }
                else
                    throw new InvalidOperationException();
            });

            return dateTime;
        }

        public async Task<bool> SetTime(DateTime timeStamp)
        {
            MsgHeader response = await Send(new MsgHeader(MsgType.MT_TIME_SET, sessionId, 4),
                async () =>
            {
                await channel.Write((UInt32)((DateTimeOffset)timeStamp).ToUnixTimeSeconds());
            });

            if (response.Type == MsgType.MT_AOK)
                return true;

            return false;
        }

        public async IAsyncEnumerable<ThreadInfo> GetThreads()
        {
            MsgHeader header = await Send(new MsgHeader(MsgType.MT_THREADS_GET, sessionId, 0));

            for (int i = 0; i < header.Size /14; i++)
            {
                yield return await channel.Read<ThreadInfo>();
            }
        }

        public async Task<bool> StartThread(string programName)
        {
            byte[] byteName = encoding.GetBytes(programName);

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_THREADS_START, sessionId, (short)byteName.Length), 
                async () =>
            {
                await channel.Write(byteName);
            });

            if (response.Type == MsgType.MT_AOK)
                return true;

            return false;
        }

        public async Task<bool> KillThread(int threadId)
        {
            MsgHeader response = await Send(new MsgHeader(MsgType.MT_THREADS_KILL, sessionId, 1), 
                async () =>
            {
                await channel.Write((byte)threadId);
            });

            if (response.Type == MsgType.MT_AOK)
                return true;

            return false;
        }

        public async Task<Storage?> GetStorage(StorageTypes storageType)
        {
            Storage? storage = new Storage();

            await Send(new MsgHeader(MsgType.MT_STORAGE_GET, sessionId, sizeof(StorageTypes)),
                async () =>
            {
                await channel.Write(storageType);
            },
                async response =>
            {
                if(response.Type == MsgType.MT_NOK)
                {
                    storage = null;
                    return;
                }

                storage.Info = await channel.Read<StorageInfo>();
                List<string> entries = new List<string>();

                for (int i = 0; i < (response.Size - sizeof(StorageTypes)) / 5; i++)
                {
                    byte[] entrySid = await channel.Read(5);
                    entries.Add(encoding.GetString(entrySid));
                }

                storage.Entries = entries.AsReadOnly();
            });

            return storage;
        }

        public async Task<bool> AddEntry(StorageTypes storageType, string name, byte[] content = null)
        {
            byte[] byteName = encoding.GetBytes(name);

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_ADD, sessionId, (short)(sizeof(StorageTypes) + byteName.Length)), 
                async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
            });

            if (response.Type == MsgType.MT_NOK)
                return false;

            if(content != null)
            {
                return await WriteToEntry(storageType, name, content);
            }

            return true;
        }

        public async Task<bool> RemoveEntry(StorageTypes storageType, string name)
        {
            byte[] byteName = encoding.GetBytes(name);

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_REMOVE, sessionId, (short)byteName.Length), 
                async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
            });

            return response.Type == MsgType.MT_AOK;
        }

        public async Task<bool> WriteToEntry(StorageTypes storageType, string name, byte[] content)
        {
            byte[] byteName = encoding.GetBytes(name);

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_UPDATE, sessionId, (short)(sizeof(StorageTypes) + byteName.Length + content.Length)), 
                async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
                await channel.Write(content);
            });

            return response.Type == MsgType.MT_AOK;
        }

        public async Task<byte[]?> ReadEntry(StorageTypes storageType, string name, int offeset, int length)
        {
            byte[] byteName = encoding.GetBytes(name);

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_READ, sessionId, (short)(sizeof(StorageTypes) + byteName.Length + 8)),
            async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
                await channel.Write(offeset);
                await channel.Write(length);
            });

            if(response.Type == MsgType.MT_ENTRY_CONTENT)
            {
                return await channel.Read(response.Size);
            }

            return null;
        }

        public async Task<bool> WriteToEntry<T>(StorageTypes storageType, string name, T content)
        {
            byte[] byteName = encoding.GetBytes(name);

            int size = 0;
            unsafe { size = sizeof(T); }

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_UPDATE, sessionId, (short)(sizeof(StorageTypes) + byteName.Length + size)),
            async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
                await channel.Write(content);
            });

            return response.Type == MsgType.MT_AOK;
        }

        public async Task<T> ReadEntry<T>(StorageTypes storageType, string name) where T : new()
        {
            byte[] byteName = encoding.GetBytes(name);

            int size = 0;
            unsafe { size = sizeof(T); }

            MsgHeader response = await Send(new MsgHeader(MsgType.MT_ENTRY_READ, sessionId, (short)(sizeof(StorageTypes) + byteName.Length + 8)),
            async () =>
            {
                await channel.Write(storageType);
                await channel.Write(byteName);
                await channel.Write(0);
                await channel.Write(size);
            });

            if (response.Type == MsgType.MT_ENTRY_CONTENT)
            {
                return await channel.Read<T>();
            }

            return new T();
        }

        public async Task<float?> GetSensorValue(string name)
        {
            byte[] byteName = encoding.GetBytes(name);
            MsgHeader response = await Send(new MsgHeader(MsgType.MT_GET_SENS_VAL, sessionId, (short)byteName.Length),
            async () =>
            {
                await channel.Write(byteName);
            });

            if (response.Type == MsgType.MT_SENS_VAL)
            {
                return await channel.Read<float>();
            }

            return null;
        }
    }
}