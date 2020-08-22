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
   public class BlockifiedModule
   {
      #region Types / Constants

      private enum Error
      {
         DuplicateFunctionName,
         InternalErrorUnknownBlockType,
         MultipleMains
      }

      #endregion

      #region Private Fields

      private Dictionary<string, FunctionBlock> functions = new Dictionary<string, FunctionBlock>();
      private Dictionary<string, InlineBlock> inlines = new Dictionary<string, InlineBlock>();

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

      /// <summary>
      /// Adds the given block to the module
      /// </summary>
      /// <param name="block"></param>
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
            var functionBlock = block as FunctionBlock;
            string name = functionBlock.Name;
            if (functions.ContainsKey(name))
               throw new CompileException(Error.DuplicateFunctionName);
            functions.Add(name, functionBlock);
         }
         else if (block is InlineBlock)
         {
            var inlineBlock = block as InlineBlock;
            string name = inlineBlock.Name;
            inlines.Add(name, inlineBlock);
         }
         else
         {
            throw new CompileException(Error.InternalErrorUnknownBlockType);
         }
      }

      /// <summary>
      /// Gets the block associated with the given identifier
      /// </summary>
      /// <param name="identifier"></param>
      /// <returns></returns>
      public Block GetBlock(string identifier)
      {
         if (inlines.ContainsKey(identifier))
            return inlines[identifier];
         else if (functions.ContainsKey(identifier))
            return functions[identifier];
         else
            return null;
      }

      #endregion
   }
}
