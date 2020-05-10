using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev
{
   class SourceModule
   {
      public SourceModule(string text)
      {
         this.ModuleText = text;
      }

      public string ModuleText
      {
         get;
         private set;
      }
   }
}
