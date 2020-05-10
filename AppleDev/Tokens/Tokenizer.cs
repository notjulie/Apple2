using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   class Tokenizer
   {
      /// <summary>
      /// Creates a TokenizedModule for the SourceModule
      /// </summary>
      /// <param name="sourceModule"></param>
      /// <returns></returns>
      public TokenizedModule Tokenize(SourceModule sourceModule)
      {
         TokenizedModule result = new TokenizedModule();

         using (TextReader reader = sourceModule.OpenReader())
         {
            for (; ; )
            {
               string line = reader.ReadLine();
               if (line == null)
                  return result;

               TokenizeLine(line);
            }
         }
      }

      private void TokenizeLine(string line)
      {
         throw new NotImplementedException("Tokenizer.TokenizeLine");
      }
   }
}
