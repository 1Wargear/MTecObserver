using CommandLine.Text;
using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.DTOs;
using BetterConsoleTables;

namespace MTOCLI.ConnectVerbs
{
    [Verb("listentries", HelpText = "Lists all entries in a storage")]
    internal class ListStorageEntries : IConnectionCommand
    {
        [Option('s', "storage", Required = true, HelpText = "The storage to add the entry to")]
        public StorageTypes StorageType { get; set; }

        public async Task Execute(IOCom oCom)
        {
            Storage? store = await oCom.GetStorage(StorageType);

            if(store == null)
            {
                Console.WriteLine("Storage not Found");
                return;
            }

            Table tblInfo = new Table("KEY", "VALUE") { Config = TableConfiguration.MySql() };
            tblInfo.AddRow("Type", store.Info.Type);
            tblInfo.AddRow("Driver", store.Info.Driver);
            tblInfo.AddRow("Size", store.Info.Size);
            Console.WriteLine(tblInfo.ToString());

            Table tblEntires = new Table("NAME" ) { Config = TableConfiguration.MySql() };
            foreach (var item in store.Entries)
            {
                tblEntires.AddRow(item);
            }
            Console.WriteLine(tblEntires.ToString());
        }
    }
}
