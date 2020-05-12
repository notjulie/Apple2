using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   /// <summary>
   /// Base class representing a token in the source
   /// </summary>
   class Token
   {
      #region Types / Constants

      private enum Error
      {
         InternalErrorEmptyToken
      }

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class Token
      /// </summary>
      /// <param name="tokenInfo"></param>
      public Token(TokenInfo tokenInfo)
      {
         this.TokenInfo = tokenInfo;

         if (TokenInfo.Text.Length == 0)
            throw new CompileException(Error.InternalErrorEmptyToken);
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets information about the token
      /// </summary>
      public TokenInfo TokenInfo
      {
         get;
         private set;
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Returns a string representation of the object.
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return TokenInfo.Text;
      }

      #endregion
   }
}
