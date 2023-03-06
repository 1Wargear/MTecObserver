using CommandLine.Text;
using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.DTOs;
using System.Threading;
using BetterConsoleTables;

namespace MTOCLI.ConnectVerbs
{
    [Verb("getthreads", HelpText = "Displays all executing programms on the Plattform")]
    internal class GetThreads : IConnectionCommand
    {
        public async Task Execute(IOCom oCom)
        {
            Table table = new Table("ID", "NAME", "STATUS", "SIZE");
            table.Config = TableConfiguration.MySql();

            await foreach(ThreadInfo thread in oCom.GetThreads())
            {
                table.AddRow(thread.ThreadId, thread.Name.ToString(), thread.Status, thread.StackSize);
            }

            Console.WriteLine(table.ToString());
        }
    }
}
