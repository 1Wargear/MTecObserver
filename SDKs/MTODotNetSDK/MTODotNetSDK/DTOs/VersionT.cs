using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct VersionT
    {
        public byte Major;
        public byte Minor;
        public byte Patch;
        public byte Build;

        public VersionT(byte major, byte minor, byte patch, byte build) : this()
        {
            Major = major;
            Minor = minor;
            Patch = patch;
            Build = build;
        }

        public override string ToString()
        {
            return $"{Major}.{Minor}.{Patch}.{Build}";
        }
    }
}