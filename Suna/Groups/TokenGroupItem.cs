using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Suna.Link;
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

      /// <summary>
      /// Compiles the item in the given context
      /// </summary>
      /// <param name="linkContext"></param>
      public override void Compile(LinkContext linkContext)
      {
         // in general, tokens shouldn't compile, but let the token handle it
         // anyway since they are self-aware polymorphic objects
         Token.Compile(linkContext);
      }

      public override string ToString()
      {
         return Token.ToString();
      }

      #endregion
   }
}
