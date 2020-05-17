using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Link;
using Suna.Tokens;

namespace Suna.Groups
{
   /// <summary>
   /// Represents content within curly brackets
   /// </summary>
   class CurlyBracketGroup : Group
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class CurlyBracketGroup
      /// </summary>
      private CurlyBracketGroup()
      {
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Reads a CurlyBracketGroup from the given enumerator 
      /// </summary>
      /// <param name="enumerator"></param>
      /// <returns></returns>
      public static CurlyBracketGroup Read(IEnumerator<Token> enumerator)
      {
         var result = new CurlyBracketGroup();
         result.ReadGroupUntil(
            enumerator,
            (Token token) => {
               var symbolToken = token as SymbolToken;
               return symbolToken!=null && symbolToken.Symbol==Symbol.RightCurlyBracket;
            }
            );
         return result;
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Compiles this object in the given context
      /// </summary>
      /// <param name="linkContext"></param>
      public override void Compile(LinkContext linkContext)
      {
         // grab an enumerator
         IEnumerator<GroupItem> items = Items.GetEnumerator();

         // enumerate
         while (items.MoveNext())
         {
            throw new NotImplementedException();
         }
      }

      /// <summary>
      /// Returns a string representation of this object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return "{" + base.ToString() + "}";
      }

      #endregion
   }
}
