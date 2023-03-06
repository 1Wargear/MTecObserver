using CommandLine.Text;
using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.DTOs;
using MTODotNetSDK.Interfaces;

namespace MTOCLI.ConnectVerbs
{
    [Verb("rementry", HelpText = "Removes entry from storage")]
    internal class RemoveStorageEntry : IConnectionCommand
    {
        [Option('s', "storage", Required = true, HelpText = "The storage to add the entry to")]
        public StorageTypes StorageType { get; set; }

        [Option('n', "name", Required = true, HelpText = "The name of the entry")]
        public string Name { get; set; }

        public async Task Execute(IOCom oCom)
        {
            if (await oCom.RemoveEntry(StorageType, Name))
                Console.WriteLine($"Entry {Name} was removed from {StorageType} successfully");
            else
                Console.WriteLine($"Failed to remove {Name} from {StorageType}");
        }
    }
}
