using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   class ImportBlock : Block
   {
      /// <summary>
      /// Initializes a new instance of class ImportBlock
      /// </summary>
      /// <param name="tokens"></param>
      public ImportBlock(Token[] tokens)
         : base(tokens)
      {
         Path = ((StringToken)tokens[1]).Value;
      }

      public string Path
      {
         get;
         private set;
      }

      public override void AddToModule(BlockifiedModule blockifiedModule)
      {
         throw new NotImplementedException();
      }
   }
}
