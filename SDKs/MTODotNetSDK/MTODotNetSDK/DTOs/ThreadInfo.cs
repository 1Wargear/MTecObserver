using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    public enum ThreadStatus
    {
        TS_DONE = 0,
        TS_WAITING = 1,
        TS_RUNNING = 2
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ThreadInfo
    {
        public byte ThreadId;
        public uint StackSize;
        public ThreadStatus Status;
        public StaticId Name;
    }
}
