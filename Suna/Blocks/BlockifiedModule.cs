using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   /// <summary>
   /// A collection of blocks of code; a blockification is just a process of splitting a
   /// source file into manageable sections; functions, inlines, etc.
   /// </summary>
   class BlockifiedModule
   {
      #region Types / Constants

      private enum Error
      {
         InternalErrorUnknownBlockType,
         MultipleMains
      }

      #endregion

      #region Private Fields

      private List<Block> functions = new List<Block>();
      private List<Block> inlines = new List<Block>();

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the "main" block
      /// </summary>
      public MainBlock Main
      {
         get;
         private set;
      } = null;

      #endregion

      #region Public Methods

      public void Add(Block block)
      {
         if (block is MainBlock)
         {
            if (Main != null)
               throw new CompileException(Error.MultipleMains);
            Main = (MainBlock)block;
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

      #endregion
   }
}
