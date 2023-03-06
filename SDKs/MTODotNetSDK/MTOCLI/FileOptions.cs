using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI
{
    [Verb("file", HelpText = "Create, View and Edit MTO files")]
    internal class FileOptions
    {
        [Option('f', "file", Required = true, HelpText = "The name of the file")]
        public string FileName { get; set; }
    }
}
