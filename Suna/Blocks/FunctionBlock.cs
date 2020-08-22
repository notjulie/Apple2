using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Blocks
{
   /// <summary>
   /// A block of tokens representing a function
   /// </summary>
   public class FunctionBlock : Block
   {
      /// <summary>
      /// Initializes a new instance of class FunctionBlock
      /// </summary>
      /// <param name="tokens"></param>
      public FunctionBlock(Token[] tokens)
         :base(tokens)
      {
         //TODO... set Name property
         throw new NotImplementedException();
      }

      /// <summary>
      /// Gets the name
      /// </summary>
      public string Name
      {
         get;
         private set;
      }

      /// <summary>
      /// Adds this block to the given module
      /// </summary>
      /// <param name="blockifiedModule"></param>
      public override void AddToModule(BlockifiedModule blockifiedModule)
      {
         Contract.Requires(blockifiedModule != null);

         blockifiedModule.AddFunction(this);
      }
   }
}
