using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace MTODotNetSDK.Interfaces
{
    public interface IComChannel : IDisposable, IAsyncDisposable
    {
        bool IsConnected { get; }

        IAsyncEnumerable<string> GetAvailableDevices();

        Task Write(byte[] buffer);
        Task<byte[]> Read(int length);
    }
}
