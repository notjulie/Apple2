using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   class TokenizedModule
   {
      private List<Token> tokens = new List<Token>();

      public void Add(Token token)
      {
         tokens.Add(token);
      }
   }
}
