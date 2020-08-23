using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Tokens
{
   /// <summary>
   /// Identifier token
   /// </summary>
   public class IdentifierToken : Token
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class IdentifierToken
      /// </summary>
      /// <param name="identifier"></param>
      public IdentifierToken(string identifier)
         : base(new TokenInfo(identifier))
      {
         this.Identifier = identifier;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the identifier string
      /// </summary>
      public string Identifier
      {
         get;
         private set;
      }

      /// <summary>
      /// Gets the list of allowed characters for an identifier
      /// </summary>
      public static ReadOnlyCollection<char> IdentifierCharacters
      {
         get;
      } = new ReadOnlyCollection<char>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789".ToCharArray());

      #endregion
   }
}
