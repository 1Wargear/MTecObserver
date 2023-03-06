using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    [Verb("disconnect", aliases: new string[]{ "quit", "exit"}, HelpText = "Disconnects from the device")]
    public class Disconnect : IConnectionCommand
    {
        public async Task Execute(IOCom oCom)
        {
            await oCom.DisposeAsync();
        }
    }
}
