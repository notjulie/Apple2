using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class CurlyBracketGroup : Group
   {
      public static CurlyBracketGroup Read(IEnumerator<Token> enumerator)
      {
         var result = new CurlyBracketGroup();
         result.ReadGroupUntil(
            enumerator,
            (Token token) => {
               var symbolToken = token as SymbolToken;
               return symbolToken!=null && symbolToken.Symbol==Symbol.RightCurlyBracket;
            }
            );
         return result;
      }
   }
}
