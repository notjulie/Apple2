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
      #region Constructor

      public TokenGroupItem(Token token)
      {
         this.Token = token;
      }

      #endregion

      #region Public Properties

      public Token Token
      {
         get;
         private set;
      }

      #endregion

      #region Base Class Overrides

      public override bool IsIdentifier
      {
         get
         {
            return Token is IdentifierToken;
         }
      }

      public override string ToString()
      {
         return Token.ToString();
      }

      #endregion
   }
}
