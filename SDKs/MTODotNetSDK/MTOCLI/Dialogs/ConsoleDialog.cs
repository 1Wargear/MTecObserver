using MTODotNetSDK.DTOs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.Dialogs
{
    internal static class ConsoleDialog
    {
        public static T OpenDialog<T>(T instance)
        {
            object obj = instance;

            Console.WriteLine($"{typeof(T).Name}:");
            foreach (var item in typeof(T).GetFields())
            {
                Console.Write($"{item.Name} ({item.GetValue(obj).ToString().Trim()}): ");
                string line = Console.ReadLine();
                
                if(item.FieldType == typeof(StaticId))
                {
                    item.SetValue(obj, (StaticId)line);
                }
                else
                {
                    item.SetValue(obj, Convert.ChangeType(line, item.FieldType));
                }
            }

            return (T)obj;
        }
    }
}
