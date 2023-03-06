using MTODotNetSDK.DTOs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTODotNetSDK.Exceptions
{
    public class HSFailedException : Exception
    {
        public HSFailedException(VersionT version) : base(version.ToString())
        {
            
        }
    }
}
