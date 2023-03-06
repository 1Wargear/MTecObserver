using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct SensorDefinition
    {
        public StaticId Name;
        public StaticId Driver;

        public byte InterfaceType;
        public short Address;
    }
}
