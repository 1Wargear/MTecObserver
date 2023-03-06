using CommandLine;
using MTODotNetSDK.DTOs;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    [Verb("addentry", HelpText = "Adds new entry to storage")]
    internal class AddSorageEntry : IConnectionCommand
    {
        [Option('s', "storage", Required = true, HelpText = "The storage to add the entry to")]
        public StorageTypes StorageType { get; set; }

        [Option('n', "name", Required = true, HelpText = "The name of the entry")]
        public string Name { get; set; }

        public async Task Execute(IOCom oCom)
        {
            if (await oCom.AddEntry(StorageType, Name, null))
                Console.WriteLine($"Entry {Name} was added to {StorageType} successfully");
            else
                Console.WriteLine($"Failed to add {Name} to {StorageType}");
        }
    }
}
