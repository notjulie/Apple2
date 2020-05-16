using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class Group
   {
      private List<Token> tokens = new List<Token>();

      /// <summary>
      /// Initializes a new instance of class Group
      /// </summary>
      /// <param name="token"></param>
      public Group(Token token)
      {
         tokens.Add(token);
      }
   }
}
