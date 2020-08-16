using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Tokens
{
   class StringToken : Token
   {
      private enum Error
      {
         UnterminatedStringLiteral
      }

      public StringToken(TokenInfo tokenInfo, string s)
         :base(tokenInfo)
      {
         this.Value = s;
      }

      public string Value
      {
         get;
         private set;
      }

      public static Token GetStringToken(ref string text)
      {
         // do this simple for now
         int end = text.IndexOf('"', 1);
         if (end < 0)
            throw new CompileException(Error.UnterminatedStringLiteral);

         string s = text.Substring(1, end - 1);
         text = text.Substring(end + 1);
         return new StringToken(new TokenInfo('"' + s + '"'), s);
      }
   }
}
