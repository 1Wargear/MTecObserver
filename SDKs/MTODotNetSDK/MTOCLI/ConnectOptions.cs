using BetterConsoleTables;
using CommandLine;
using MTOCLI.ConnectVerbs;
using MTODotNetSDK.Channels;
using MTODotNetSDK.Interfaces;
using MTODotNetSDK;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Configuration;
using System.Text;

namespace MTOCLI
{
    [Verb("connect", HelpText = "Connects to MTec-Observer session")]
    internal class ConnectOptions
    {
        [Option('c', "channel", Required = true, HelpText = "The channel used to comunikate with OCOM")]
        public string ChannelName { get; set; }

        [Option('d', "device", Required = false, HelpText = "The name of the device to conatct", Group = "Operation")]
        public string DeviceId { get; set; }

        [Option('a', "available", Required = false, HelpText = "Displays all available devices to the channel", Group = "Operation")]
        public bool ShowAvailable { get; set; }

        public async Task Execute(Regex commandRegex, IConfiguration config, ILoggerFactory loggerFactory)
        {
            var logger = loggerFactory.CreateLogger<ConnectOptions>();

            try
            {
                IComChannel channel = new SerialPortChannel(DeviceId, config);

                if (ShowAvailable)
                {
                    Table deviceTable = new Table("Available Devices".ToUpper()) { Config = TableConfiguration.MySql() };
                    await foreach (var item in channel.GetAvailableDevices())
                    {
                        deviceTable.AddRow(item);
                    }
                    Console.WriteLine(deviceTable.ToString());
                    return;
                }

                IOCom ocom = new OComClient(channel);
                var info = await ocom.Connect();

                Table sysInfoTable = new Table(DeviceId.ToUpper(), "VALUES") { Config = TableConfiguration.MySql() };
                //sysInfoTable.AddRow("Name", info.Name);
                sysInfoTable.AddRow("Version", info.Version.ToString());
                sysInfoTable.AddRow("MTO-Version", info.MTOVersion.ToString());
                sysInfoTable.AddRow("OCOM-Version", info.OCOMVersion.ToString());
                sysInfoTable.AddRow("MTO-LIGHT", Convert.ToBoolean(info.MTOLight));
                Console.WriteLine(sysInfoTable.ToString());

                while (channel.IsConnected)
                {
                    Console.Write($"{DeviceId}> ");
                    Parser.Default.ParseArguments<GetDeviceTime, SetDeviceTime, GetThreads, StartThread, KillThread,
                            Disconnect, AddSorageEntry, ListStorageEntries, RemoveStorageEntry, EditEntry, GetSensorValue>(
                            commandRegex.Matches(Console.ReadLine()).Cast<Match>().Select(m => m.Value))
                        .WithParsed(o =>
                        {
                            try
                            {
                                ((IConnectionCommand)o).Execute(ocom).Wait();
                            }
                            catch (Exception ex)
                            {
                                logger.LogError(ex.Message);
                            }
                        });
                }

                Console.WriteLine("Disconnected");
            }
            catch (Exception ex)
            {
                logger.LogError(ex.Message);
            }
        }
    }
}
