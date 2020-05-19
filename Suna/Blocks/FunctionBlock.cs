using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Blocks
{
   class FunctionBlock : Block
   {
      public FunctionBlock(Token[] tokens)
         :base(tokens)
      {
         //TODO... set Name property
         throw new NotImplementedException();
      }

      public string Name
      {
         get;
         private set;
      }
   }
}
