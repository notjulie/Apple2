using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Tokens
{
   /// <summary>
   /// Represents information about a token
   /// </summary>
   public struct TokenInfo : IEquatable<TokenInfo>
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class TokenInfo
      /// </summary>
      /// <param name="text"></param>
      public TokenInfo(string text)
      {
         this.Text = text;
      }

      #endregion

      #region Properties

      /// <summary>
      /// Gets the token's text
      /// </summary>
      public string Text
      {
         get;
         private set;
      }

      #endregion

      #region Base class overrides

      /// <summary>
      /// Compares this instance to another object
      /// </summary>
      /// <param name="obj"></param>
      /// <returns></returns>
      public override bool Equals(object obj)
      {
         if (obj is TokenInfo)
            return Equals((TokenInfo)obj);
         else
            return false;
      }

      /// <summary>
      /// Returns a hash code for the object
      /// </summary>
      /// <returns></returns>
      public override int GetHashCode()
      {
         return Text.GetHashCode();
      }

      #endregion

      #region IEquatable implementation

      /// <summary>
      /// Compares this instance to another
      /// </summary>
      /// <param name="other"></param>
      /// <returns></returns>
      public bool Equals(TokenInfo other)
      {
         return Text == other.Text;
      }

      #endregion

      #region Operator overloads

      /// <summary>
      /// Equality operator
      /// </summary>
      /// <param name="a"></param>
      /// <param name="b"></param>
      /// <returns></returns>
      public static bool operator==(TokenInfo a, TokenInfo b)
      {
         return a.Equals(b);
      }

      /// <summary>
      /// Inequality operator
      /// </summary>
      /// <param name="a"></param>
      /// <param name="b"></param>
      /// <returns></returns>
      public static bool operator !=(TokenInfo a, TokenInfo b)
      {
         return !(a == b);
      }

      #endregion
   }
}
