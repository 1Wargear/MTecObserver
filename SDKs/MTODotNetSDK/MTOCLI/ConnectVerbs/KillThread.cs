using CommandLine.Text;
using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    [Verb("killthread", HelpText = "Starts a new program instance on the Plattform")]
    internal class KillThread : IConnectionCommand
    {
        [Option('t', "thread", Required = true, HelpText = "The id of the Thread to kill")]
        public int ThreadId { get; set; }

        public async Task Execute(IOCom oCom)
        {
            if(await oCom.KillThread(ThreadId))
                Console.WriteLine($"Thread {ThreadId} killed successfully");
            else
                Console.WriteLine($"Thread {ThreadId} failed to kill");
        }
    }
}
