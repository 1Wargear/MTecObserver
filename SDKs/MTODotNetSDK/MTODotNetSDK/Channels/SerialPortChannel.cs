using Microsoft.Extensions.Configuration;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.Channels
{
    public class SerialPortChannel : IComChannel
    {
        private readonly SerialPort serialPort;

        public bool IsConnected { get; private set; }

        public SerialPortChannel(string portName, IConfiguration configuration)
        {
            if (String.IsNullOrEmpty(portName))
                return;

            serialPort = new SerialPort()
            {
                PortName = portName,
                BaudRate = 9600,
                ReadTimeout = 10000,
                WriteTimeout = 10000,
            };

            serialPort.Open();
            IsConnected = true;
        }

        public SerialPortChannel(IConfiguration configuration)
        {

        }

        public void Dispose()
        {
            IsConnected = false;
            serialPort.Close();
            serialPort.Dispose();
        }

        public ValueTask DisposeAsync()
        {
            Dispose();
            return ValueTask.CompletedTask;
        }

        public Task<byte[]> Read(int length)
        {
            byte[] buffer = new byte[length];
            for (int i = 0; i < length; i++)
            {
                serialPort.Read(buffer, i, 1);
            }
            return Task.FromResult(buffer);
        }

        public Task Write(byte[] buffer)
        {
            serialPort.Write(buffer, 0, buffer.Length);
            return Task.CompletedTask;
        }

        public async IAsyncEnumerable<string> GetAvailableDevices()
        {
            foreach (var item in SerialPort.GetPortNames())
            {
                yield return item;
            }
        }
    }
}
