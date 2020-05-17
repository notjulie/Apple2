using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   /// <summary>
   /// Represents an item in our grouped view of the code; this is just my way
   /// of dealing with the hierachical nature of code.  A GroupItem can be:
   ///    - a token
   ///    - a group of items bounded by brackets or parentheses
   ///    - other things as I see fit
   /// </summary>
   class GroupItem
   {
      #region Private Fields

      private Token token;
      private List<GroupItem> childItems;

      #endregion

      #region Constructor

      public GroupItem(Token token)
      {
         this.token = token;
      }

      public GroupItem(ICollection<GroupItem> items)
      {
         childItems = new List<GroupItem>(items);
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Returns a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         if (token != null)
         {
            return token.ToString();
         }
         else
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
      }

      #endregion
   }
}
