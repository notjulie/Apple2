using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Groups;

namespace Suna.Link
{
   class LinkContext
   {
      public LinkContext(LinkedModule module)
      {
         this.LinkedModule = module;
      }

      public Groupifier Groupifier
      {
         get;
      } = new Groupifier();

      public LinkedModule LinkedModule
      {
         get;
         private set;
      }
   }
}
