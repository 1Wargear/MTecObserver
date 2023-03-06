using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly record struct MsgHeader
    (
        MsgType Type,
        ushort SessionId,
        short Size
    );
}
