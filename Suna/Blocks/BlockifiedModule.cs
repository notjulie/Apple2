using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   class BlockifiedModule
   {
      private List<Block> blocks = new List<Block>();

      public void Add(Block block)
      {
         blocks.Add(block);
      }
   }
}
