using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class TokenGroupItem : GroupItem
   {
      public TokenGroupItem(Token token)
      {
         this.Token = token;
      }

      public Token Token
      {
         get;
         private set;
      }

      public override string ToString()
      {
         return Token.ToString();
      }
   }
}
