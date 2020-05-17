using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class Group : GroupItem
   {
      #region Types / Constants

      private enum Error
      {
         MissingClosingSymbol,
         UnexpectedSymbol
      }

      #endregion 

      #region Private Fields

      private List<GroupItem> childItems = new List<GroupItem>();

      #endregion

      #region Public Methods

      public void Add(GroupItem item)
      {
         childItems.Add(item);
      }

      #endregion

      #region Protected Methods

      protected void ReadGroupUntil(IEnumerator<Token> enumerator, Func<Token,bool> isGroupEnd)
      {
         for (; ; )
         {
            // move to the next token
            if (!enumerator.MoveNext())
               throw new CompileException(Error.MissingClosingSymbol);

            // see if it's the closing symbol we need
            if (isGroupEnd(enumerator.Current))
               return;

            // else let the token read the next group item from the enumerator
            Add(enumerator.Current.ReadGroupItem(enumerator));
         }
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Returns a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         StringBuilder s = new StringBuilder();
         foreach (var item in childItems)
         {
            if (s.Length == 0)
               s.Append(' ');
            s.Append(item.ToString());
         }
         return s.ToString();
      }

      #endregion
   }
}
