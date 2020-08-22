using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   /// <summary>
   /// Represents a group of items in parentheses
   /// </summary>
   public class ParentheticGroup : Group
   {
      /// <summary>
      /// Reads a group from the given stream of tokens
      /// </summary>
      /// <param name="enumerator"></param>
      /// <returns></returns>
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

      /// <summary>
      /// Returns a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return "(" + base.ToString() + ")";
      }
   }
}
