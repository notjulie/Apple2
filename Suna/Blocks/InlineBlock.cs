using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Blocks
{
   class InlineBlock : Block
   {
      public InlineBlock(Token[] tokens)
         :base(tokens)
      {
      }
   }
}
