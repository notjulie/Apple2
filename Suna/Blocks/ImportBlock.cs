using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   /// <summary>
   /// An import of an external module
   /// </summary>
   public class ImportBlock : Block
   {
      /// <summary>
      /// Initializes a new instance of class ImportBlock
      /// </summary>
      /// <param name="tokens"></param>
      public ImportBlock(Token[] tokens)
         : base(tokens)
      {
         Contract.Requires(tokens != null);

         Path = ((StringToken)tokens[1]).Value;
      }

      /// <summary>
      /// Gets the path
      /// </summary>
      public string Path
      {
         get;
         private set;
      }

      /// <summary>
      /// Adds this instance to the given module
      /// </summary>
      /// <param name="blockifiedModule"></param>
      public override void AddToModule(BlockifiedModule blockifiedModule)
      {
         Contract.Requires(blockifiedModule != null);

         blockifiedModule.AddImport(this);
      }
   }
}
