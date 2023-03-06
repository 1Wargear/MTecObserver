using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.DTOs
{
    public class Storage
    {
        public StorageInfo Info { get; set; }
        public IReadOnlyList<string> Entries { get; set; } = new List<string>();
    }
}
