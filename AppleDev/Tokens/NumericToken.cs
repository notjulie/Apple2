using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   class NumericToken : Token
   {
      public NumericToken(TokenInfo info, long number)
         :base(info)
      {
         this.Number = number;
      }

      public long Number
      {
         get;
         private set;
      }

      public static Token GetNumericToken(ref string text)
      {
         // count how many characters are in the token
         int count;
         for (count = 0; count < text.Length; ++count)
            if (!char.IsLetterOrDigit(text[count]))
               break;

         // extract the tokenText
         string tokenText = text.Substring(0, count);
         text = text.Substring(count);

         // see what we have
         if (tokenText.StartsWith("0x", StringComparison.InvariantCulture))
            return new NumericToken(
               new TokenInfo(tokenText),
               long.Parse(tokenText.Substring(2), System.Globalization.NumberStyles.HexNumber, CultureInfo.InvariantCulture)
               );
         else
            throw new CompileException("Invalid numeric expression: " + tokenText);
      }
   }
}
