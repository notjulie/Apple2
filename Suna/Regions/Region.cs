using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Regions
{
   class Region
   {
      private StringBuilder contents = new StringBuilder();

      public void AddLine(string line)
      {
         contents.AppendLine(line);
      }
   }
}
