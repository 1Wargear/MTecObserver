using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    public enum MsgType : uint
    {
        MT_AOK = 0x00,
        MT_NOK = 0x01,
        MT_TIMEOUT = 0x02,
        MT_DISCONNECT = 0x03,
        MS_NOT_AVAILABLE = 0x04,
        MT_UNKNOWN = 0x05,

        MT_BEGIN = 0x06,
        MT_END = 0x07,

        MT_HS_INIT = 0x10,
        MT_HS_ACK = 0x11,
        MT_HS_NAK = 0x12,

        MT_THREADS_GET = 0x13,
        MT_THREAD_INFO = 0x014,
        MT_THREADS_START = 0x15,
        MT_THREADS_KILL = 0x16,

        MT_TIME_SET = 0x17,
        MT_TIME_GET = 0x18,
        MT_TIME_INFO = 0x19,

        MT_STORAGE_GET = 0x20,
        MT_STORAGE_INFO = 0x21,
        MT_ENTRY_ADD = 0x22,
        MT_ENTRY_REMOVE = 0x23,
        MT_ENTRY_UPDATE = 0x24,
        MT_ENTRY_READ = 0x25,
        MT_ENTRY_CONTENT = 0x26,

        MT_GET_SENS_VAL = 0x30,
        MT_SENS_VAL = 0x31,

        MT_ERROR = 0xFFFFFFFF
    }
}
