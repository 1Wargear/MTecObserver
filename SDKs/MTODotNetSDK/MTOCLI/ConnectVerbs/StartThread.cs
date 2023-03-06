using BetterConsoleTables;
using CommandLine.Text;
using CommandLine;
using MTODotNetSDK.DTOs;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    [Verb("startthread", HelpText = "Starts a new program instance on the Plattform")]
    internal class StartThread : IConnectionCommand
    {
        [Option('p', "program", Required = true, HelpText = "The name of the Program to start")]
        public string ProgramName { get; set; }

        public async Task Execute(IOCom oCom)
        {
            if (await oCom.StartThread(ProgramName))
                Console.WriteLine($"Done {ProgramName} started");
            else
                Console.WriteLine($"Failed to start {ProgramName}");
        }
    }
}
