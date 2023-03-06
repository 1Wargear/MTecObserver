using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SystemInfo
    {
        //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        //public byte[] Name;
        public VersionT Version;
        public VersionT MTOVersion;
        public VersionT OCOMVersion;
        public byte MTOLight;
    }
}
