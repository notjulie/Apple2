using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AppleDev.Tokens;

namespace AppleDev
{
   class Compiler
   {
      public void Compile(SourceModule sourceModule)
      {
         Tokenizer tokenizer = new Tokenizer();
         var tokenizedModule = tokenizer.Tokenize(sourceModule);

         throw new NotImplementedException();
      }
   }
}
