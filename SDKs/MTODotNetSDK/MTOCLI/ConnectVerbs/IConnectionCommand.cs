using MTODotNetSDK.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTOCLI.ConnectVerbs
{
    internal interface IConnectionCommand
    {
        Task Execute(IOCom oCom);
    }
}
