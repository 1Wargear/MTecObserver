using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct SensValFileEntry
    {
        public int TimeStamp;
        public float Value;

        public SensValFileEntry(int timeStamp, float value)
        {
            TimeStamp = timeStamp;
            Value = value;
        }
    }
}
