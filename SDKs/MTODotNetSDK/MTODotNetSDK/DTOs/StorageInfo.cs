using System.Runtime.InteropServices;

namespace MTODotNetSDK.DTOs
{
    public enum StorageTypes : ushort
    {
        ST_CONFIG = 0b00000001,
        ST_TMP = 0b00000010,
        ST_LOGS = 0b00000100,
        ST_DRIVERS = 0b00001000,
        ST_DEVICES = 0b00010000,
        ST_PROGRAMS = 0b00100000,
        ST_TASKS = 0b01000000,
        ST_VALUES = 0b10000000
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct StorageInfo
    {
        public StorageTypes Type;
        public byte Driver;
        public byte Size;
    }
}