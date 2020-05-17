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
   /// Class that groupifies tokens
   /// </summary>
   class GroupedBlock : Group
   {
      #region Public Methods

      static public GroupedBlock Read(IEnumerator<Token> tokens)
      {
         var result = new GroupedBlock();
         result.ProcessAllTokens(tokens);
         return result;
      }

      #endregion

      #region Base Class Overrides

      public override void Compile(LinkContext linkContext)
      {
         throw new NotImplementedException();
      }

      #endregion

      #region Private Methods

      private void ProcessAllTokens(IEnumerator<Token> tokens)
      {
         for (; ;)
         {
            // if we're done, all good
            if (!tokens.MoveNext())
               return;

            // let the token itself build the group item
            this.Add(tokens.Current.ReadGroupItem(tokens));
         }

         throw new NotImplementedException();
      }

      #endregion
   }
}
