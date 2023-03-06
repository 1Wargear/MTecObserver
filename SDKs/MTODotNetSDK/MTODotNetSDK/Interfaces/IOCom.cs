using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.DTOs;

namespace MTODotNetSDK.Interfaces
{
    public interface IOCom : IDisposable, IAsyncDisposable
    {
        bool IsConnected { get; }

        Task<MsgHeader> Send(MsgHeader message, Func<Task>? payloadBuilder = null, Func<MsgHeader, Task>? responseHandler = null);

        event Action<MsgHeader> OnRecived;

        Task<DateTime> GetTime();
        Task<bool> SetTime(DateTime timeStamp);

        IAsyncEnumerable<ThreadInfo> GetThreads();
        Task<bool> StartThread(string programName);
        Task<bool> KillThread(int threadId);

        Task<Storage?> GetStorage(StorageTypes storageType);
        Task<bool> AddEntry(StorageTypes storageType, string name, byte[] content = null);
        Task<bool> RemoveEntry(StorageTypes storageType, string name);
        Task<bool> WriteToEntry(StorageTypes storageType, string name, byte[] cotnent = null);
        Task<byte[]?> ReadEntry(StorageTypes storageType, string name, int offeset, int length);
        Task<bool> WriteToEntry<T>(StorageTypes storageType, string name, T content);
        Task<T> ReadEntry<T>(StorageTypes storageType, string name) where T : new();

        Task<float?> GetSensorValue(string name);

        Task<SystemInfo> Connect();
        Task Disconnect();
    }
}
