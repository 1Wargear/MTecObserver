using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    [Verb("gettime", HelpText = "Gets the current Time from the connected device")]
    internal class GetDeviceTime : IConnectionCommand
    {
        public async Task Execute(IOCom oCom)
        {
            Console.WriteLine($"Device Time: {await oCom.GetTime()}");
        }
    }
}
