using CommandLine.Text;
using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.Interfaces;
using MTODotNetSDK.DTOs;

namespace MTOCLI.ConnectVerbs
{
    [Verb("getval", HelpText = "Gets the current value of a sensor")]
    internal class GetSensorValue : IConnectionCommand
    {
        [Option('n', "name", Required = true, HelpText = "The name of the sensor")]
        public string Name { get; set; }

        public async Task Execute(IOCom oCom)
        {
            float? value = await oCom.GetSensorValue(Name);
            if (value != null)
                Console.WriteLine($"Value({Name}): {value}");
            else
                Console.WriteLine($"Failed retrive value of sensor {Name}");
        }
    }
}
