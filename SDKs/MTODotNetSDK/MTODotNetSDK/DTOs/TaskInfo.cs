using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct TaskInfo
    {
        public int Intervall;
        public StaticId Name;
        public StaticId Program;

        public TaskInfo(int intervall, StaticId name, StaticId program)
        {
            Intervall = intervall;
            Name = name;
            Program = program;
        }
    }
}
