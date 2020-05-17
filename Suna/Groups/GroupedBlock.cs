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
         result.AddAllTokens(tokens);
         return result;
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Compiles this object into the given context
      /// </summary>
      /// <param name="linkContext"></param>
      public override void Compile(LinkContext linkContext)
      {
         foreach (GroupItem item in Items)
            item.Compile(linkContext);
      }

      #endregion

      #region Private Methods

      /// <summary>
      /// Adds all tokens in the enumerator to this object
      /// </summary>
      /// <param name="tokens"></param>
      private void AddAllTokens(IEnumerator<Token> tokens)
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
