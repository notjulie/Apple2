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

      public int TokenCount
      {
         get
         {
            return tokens.Length;
         }
      }

      public Token[] GetTokenRange(int offset, int count)
      {
         Token[] result = new Token[count];
         Array.Copy(tokens, offset, result, 0, count);
         return result;
      }
   }
}
