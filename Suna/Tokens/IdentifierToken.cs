using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Tokens
{
   class IdentifierToken : Token
   {
      #region Constructor

      public IdentifierToken(string identifier)
         :base(new TokenInfo(identifier))
      {
         this.Identifier = identifier;
      }

      #endregion

      #region Public Properties

      public string Identifier
      {
         get;
         private set;
      }

      public static char[] IdentifierCharacters
      {
         get;
      } = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789".ToCharArray();

      #endregion
   }
}
