using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Link;
using Suna.Tokens;

namespace Suna.Blocks
{
   class MainBlock  : Block
   {
      public MainBlock(Token[] tokens)
         :base(tokens)
      {
      }

      public void Compile(LinkContext linkContext)
      {
         throw new NotImplementedException();
      }
   }
}
