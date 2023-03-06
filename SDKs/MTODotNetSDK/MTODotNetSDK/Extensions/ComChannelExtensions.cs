using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.Extensions
{
    internal static class ComChannelExtensions
    {
        public static async Task Write<T>(this IComChannel channel, T value)
        {
            int size = 0;
            unsafe
            {
                size = sizeof(T);
            }

            byte[] buffer = new byte[size];
            unsafe
            {
                fixed (byte* buff = buffer)
                {
                    T* ptr = &value;
                    byte* bptr = (byte*)ptr;

                    for (int i = 0; i < size; i++)
                    {
                        buff[i] = bptr[i];
                    }
                }
            }

            await channel.Write(buffer);
        }

        public static async Task<T> Read<T>(this IComChannel channel) where T : new()
        {
            int size = 0;
            unsafe
            {
                size = sizeof(T);
            }

            byte[] buffer = await channel.Read(size);

            T obj = new T();
            unsafe
            {
                fixed (byte* buff = buffer)
                {
                    T* ptr = &obj;
                    byte* bptr = (byte*)ptr;

                    for (int i = 0; i < size; i++)
                    {
                        bptr[i] = buff[i];
                    }
                }
            }

            return obj;
        }
    }
}
