using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
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
         MultipleMains
      }

      #endregion

      #region Private Fields

      private MainBlock mainBlock;
      private Dictionary<string, FunctionBlock> functions = new Dictionary<string, FunctionBlock>();
      private Dictionary<string, InlineBlock> inlines = new Dictionary<string, InlineBlock>();

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the "main" block
      /// </summary>
      public MainBlock Main
      {
         get
         {
            return mainBlock;
         }
         set
         {
            if (value != mainBlock)
            {
               if (mainBlock == null)
               {
                  mainBlock = value;
               }
               else
               {
                  throw new CompileException(Error.MultipleMains);
               }
            }
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Adds the given block to the module
      /// </summary>
      /// <param name="block"></param>
      public void Add(Block block)
      {
         Contract.Requires(block != null);

         // let the block decide how to handle this
         block.AddToModule(this);
      }

      /// <summary>
      /// Adds the given function
      /// </summary>
      /// <param name="functionBlock"></param>
      public void AddFunction(FunctionBlock functionBlock)
      {
         Contract.Requires(functionBlock != null);

         string name = functionBlock.Name;
         if (functions.ContainsKey(name))
            throw new CompileException(Error.DuplicateFunctionName);
         functions.Add(name, functionBlock);
      }

      /// <summary>
      /// Adds the given function
      /// </summary>
      /// <param name="inlineBlock"></param>
      public void AddInline(InlineBlock inlineBlock)
      {
         Contract.Requires(inlineBlock != null);

         string name = inlineBlock.Name;
         inlines.Add(name, inlineBlock);
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
