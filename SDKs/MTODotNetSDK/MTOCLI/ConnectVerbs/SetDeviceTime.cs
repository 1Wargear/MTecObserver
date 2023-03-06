using CommandLine.Text;
using CommandLine;
using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MTOCLI.ConnectVerbs
{
    [Verb("settime", HelpText = "Sets the current Time on the connected device")]
    internal class SetDeviceTime : IConnectionCommand
    {
        [Option('t', "time", Group = "Time", HelpText = "The Date and Time to set the device to")]
        public string DT { get; set; }

        [Option('u', "unix", Group = "Time", HelpText = "Unix-Time-Stamp")]
        public int Unix { get; set; }

        [Option('n', "now", Group = "Time", HelpText = "Set to current Time")]
        public bool Now { get; set; }

        public async Task Execute(IOCom oCom)
        {
            DateTime dt = DateTime.Now;

            if(!Now)
            {
                if (!String.IsNullOrEmpty(DT))
                    Convert.ToDateTime(DT);
                else
                {
                    dt = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);
                    dt = dt.AddSeconds(Unix);
                }
            }

            if (await oCom.SetTime(dt))
                Console.WriteLine($"Time was successfully set to {dt}");
            else
                Console.WriteLine($"Failed to set Time to {dt}");
        }
    }
}
