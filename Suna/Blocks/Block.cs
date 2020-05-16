using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   class Block
   {
      private Token[] tokens;

      public Block(Token[] tokens)
      {
         this.tokens = tokens;
      }

      public IEnumerable<Token> Tokens
      {
         get
         {
            return tokens;
         }
      }
   }
}
