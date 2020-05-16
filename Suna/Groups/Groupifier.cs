using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   /// <summary>
   /// Class that groupifies tokens
   /// </summary>
   static class Groupifier
   {
      static public GroupItem GroupifyTokens(IEnumerator<Token> tokens)
      {
         List<GroupItem> groupItems = new List<GroupItem>();

         for (; ;)
         {
            // if we're done, all good
            if (!tokens.MoveNext())
               return new GroupItem(groupItems);

            // let the token itself build the group
            groupItems.Add(tokens.Current.ReadGroupItem(tokens));
         }

         throw new NotImplementedException();
      }
   }
}
