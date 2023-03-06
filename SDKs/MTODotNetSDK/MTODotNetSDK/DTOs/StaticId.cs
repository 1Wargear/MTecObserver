using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct StaticId
    {
        public byte CStr1;
        public byte CStr2;
        public byte CStr3;
        public byte CStr4;
        public byte CStr5;

        public override string ToString()
        {
            byte[] bytes = new byte[] { CStr1, CStr2, CStr3, CStr4, CStr5 };

            for (int i = 0; i < bytes.Length; i++)
            {
                if (bytes[i] == 0)
                    bytes[i] = 32;
            }

            return Encoding.UTF8.GetString(bytes);
        }

        public static implicit operator StaticId(string s) 
        {
            if (s.Length < 5)
                s += new string((char)0, 5 - s.Length);

            return new StaticId()
            {
                CStr1 = (byte)s[0],
                CStr2 = (byte)s[1],
                CStr3 = (byte)s[2],
                CStr4 = (byte)s[3],
                CStr5 = (byte)s[4],
            };
        }
    }
}
