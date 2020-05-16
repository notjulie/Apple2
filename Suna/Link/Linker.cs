using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;

namespace Suna.Link
{
   class Linker
   {
      private BlockifiedModule module;

      public LinkedModule Link(BlockifiedModule module)
      {
         this.module = module;

         LinkedModule result = new LinkedModule();
         throw new NotImplementedException();
      }
   }
}
