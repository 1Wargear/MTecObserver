using CommandLine.Text;
using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MTODotNetSDK.DTOs;
using MTODotNetSDK.Interfaces;
using MTOCLI.Dialogs;

namespace MTOCLI.ConnectVerbs
{
    [Verb("editentry", HelpText = "Removes entry from storage")]
    internal class EditEntry : IConnectionCommand
    {
        [Option('s', "storage", Required = true, HelpText = "The storage to add the entry to")]
        public StorageTypes StorageType { get; set; }

        [Option('n', "name", Required = true, HelpText = "The name of the entry")]
        public string Name { get; set; }

        [Option('r', "read", Group = "mode", HelpText = "Displays the content of the entry")]
        public bool Read { get; set; }

        [Option('w', "write", Group = "mode", HelpText = "Writes the Text to the entry")]
        public string Text { get; set; }

        [Option('d', "dialog", Group = "mode", HelpText = "Opens a Dialog for the File")]
        public bool Dialog { get; set; }

        public async Task Execute(IOCom oCom)
        {
            if(Read)
            {
                byte[] buffer = await oCom.ReadEntry(StorageType, Name, 0, 255);
                if (buffer == null)
                    Console.WriteLine($"Failed to read {Name} from {StorageType}");
                else
                    Console.WriteLine(Encoding.ASCII.GetString(buffer));
            }
            else if(Dialog)
            {
                switch (StorageType)
                {
                    //case StorageTypes.ST_CONFIG:
                    //    break;
                    //case StorageTypes.ST_TMP:
                    //    break;
                    //case StorageTypes.ST_LOGS:
                    //    break;
                    //case StorageTypes.ST_DRIVERS:
                    //    break;
                    case StorageTypes.ST_DEVICES:
                        await ShowDialog<SensorDefinition>(oCom);
                        break;
                    //case StorageTypes.ST_PROGRAMS:
                    //    break;
                    case StorageTypes.ST_TASKS:
                        await ShowDialog<TaskInfo>(oCom);
                        break;
                    //case StorageTypes.ST_VALUES:
                    //    break;
                    default:
                        Console.WriteLine("No Dialog available");
                        break;
                }
            }
            else
            {
                byte[] buffer = Encoding.ASCII.GetBytes(Text);
                if(await oCom.WriteToEntry(StorageType, Name, buffer))
                    Console.WriteLine($"Entry {Name} in {StorageType} was updated successfully");
                else
                    Console.WriteLine($"Failed to update {Name} in {StorageType}");
            }
        }

        private async Task ShowDialog<T>(IOCom oCom) where T : new()
        {
            T buffer = new T();
            //T buffer = await oCom.ReadEntry<T>(StorageType, Name);
            //if (buffer == null)
            //{
            //    Console.WriteLine($"Failed to read {Name} from {StorageType}");
            //    return;
            //}

            buffer = ConsoleDialog.OpenDialog<T>(buffer);

            if (await oCom.WriteToEntry<T>(StorageType, Name, buffer))
                Console.WriteLine($"Entry {Name} in {StorageType} was updated successfully");
            else
                Console.WriteLine($"Failed to update {Name} in {StorageType}");
        }

        //editentry -s ST_CONFIG -n TESTB -r
    }
}
