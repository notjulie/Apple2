using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class ParentheticGroup : Group
   {
      public static ParentheticGroup Read(IEnumerator<Token> enumerator)
      {
         var result = new ParentheticGroup();
         result.ReadGroupUntil(
            enumerator,
            (Token token) => {
               var symbolToken = token as SymbolToken;
               return symbolToken != null && symbolToken.Symbol == Symbol.RightParen;
            }
            );
         return result;
      }

      public override string ToString()
      {
         return "(" + base.ToString() + ")";
      }
   }
}
