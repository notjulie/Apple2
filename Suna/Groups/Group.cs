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

      #region Public Properties

      /// <summary>
      /// Gets the items
      /// </summary>
      public IEnumerable<GroupItem> Items
      {
         get
         {
            return childItems;
         }
      }

      #endregion

      #region Public Methods

      public void Add(GroupItem item)
      {
         childItems.Add(item);
      }

      #endregion

      #region Protected Methods

      /// <summary>
      /// Creates a group by iterating throw the given list of tokens until a token
      /// matching the isGroupEnd delegate is found
      /// </summary>
      /// <param name="enumerator"></param>
      /// <param name="isGroupEnd"></param>
      protected void ReadGroupUntil(IEnumerator<Token> enumerator, Func<Token,bool> isGroupEnd)
      {
         // scan until we hit the end
         for (; ; )
         {
            // move to the next token
            if (!enumerator.MoveNext())
               throw new CompileException(Error.MissingClosingSymbol);

            // see if it's the closing symbol we need
            if (isGroupEnd(enumerator.Current))
               break;

            // else let the token read the next group item from the enumerator
            Add(enumerator.Current.ReadGroupItem(enumerator));
         }

         // look through the result and combine things
         // look for the invoke pattern
         for (int i=0; i<childItems.Count-1; ++i)
         {
            if (childItems[i].IsIdentifier && childItems[i+1] is ParentheticGroup)
            {
               var invokeGroup = new InvokeGroup(childItems[i], childItems[i + 1]);
               childItems.RemoveAt(i + 1);
               childItems[i] = invokeGroup;
            }
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
