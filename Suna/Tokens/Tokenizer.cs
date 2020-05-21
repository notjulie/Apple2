using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Regions;

namespace Suna.Tokens
{
   class Tokenizer
   {
      /// <summary>
      /// Creates a TokenizedModule for the SourceModule
      /// </summary>
      /// <param name="sunaRegion"></param>
      /// <returns></returns>
      public void Tokenize(SunaRegion sunaRegion)
      {
         using (TextReader reader = sunaRegion.OpenReader())
         {
            for (; ; )
            {
               string line = reader.ReadLine();
               if (line == null)
                  return;

               TokenizeLine(line);
            }
         }
      }

      public TokenizedModule TokenizedModule
      {
         get;
      } = new TokenizedModule();

      private void TokenizeLine(string line)
      {
         // process until we've used up the whole line
         for (; ; )
         {
            // trim whitespace
            line = line.Trim();
            if (line.Length == 0)
               return;

            // switch according to first character
            char firstCharacter = line[0];
            if (char.IsLetter(firstCharacter))
               TokenizedModule.Add(GetWordToken(ref line));
            else if (SymbolToken.IsSymbolCharacter(firstCharacter))
               TokenizedModule.Add(SymbolToken.GetSymbolToken(ref line));
            else if (char.IsDigit(firstCharacter))
               TokenizedModule.Add(NumericToken.GetNumericToken(ref line));
            else
               throw new NotImplementedException("Invalid token start character: " + firstCharacter);
         }
      }

      private static Token GetWordToken(ref string line)
      {
         string tokenString;

         // find the position of the first character that's not allowed to be part
         // of a word
         int endPosition = -1;
         for (int i=1; i<line.Length; ++i)
         {
            if (!IdentifierToken.IdentifierCharacters.Contains(line[i]))
            {
               endPosition = i;
               break;
            }
         }
         if (endPosition < 0)
         {
            tokenString = line;
            line = string.Empty;
         }
         else
         {
            tokenString = line.Substring(0, endPosition);
            line = line.Substring(endPosition);
         }

         // see if it's a keyword
         Keyword keyword;
         if (Enum.TryParse(tokenString, out keyword))
            return new KeywordToken(keyword);
         else
            return new IdentifierToken(tokenString);
      }
   }
}
