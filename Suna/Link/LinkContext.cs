using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Link
{
   class LinkContext
   {
      public LinkContext(LinkedModule module)
      {
         this.LinkedModule = module;
      }

      public LinkedModule LinkedModule
      {
         get;
         private set;
      }
   }
}
