using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   class BlockifiedModule
   {
      private enum Error
      {
         InternalErrorUnknownBlockType,
         MultipleMains
      }

      private MainBlock main = null;
      private List<Block> functions = new List<Block>();
      private List<Block> inlines = new List<Block>();

      public void Add(Block block)
      {
         if (block is MainBlock)
         {
            if (main != null)
               throw new CompileException(Error.MultipleMains);
            main = (MainBlock)block;
         }
         else if (block is FunctionBlock)
         {
            functions.Add(block);
         }
         else if (block is InlineBlock)
         {
            inlines.Add(block);
         }
         else
         {
            throw new CompileException(Error.InternalErrorUnknownBlockType);
         }
      }
   }
}
