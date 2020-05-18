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
   /// Represents an "invocation", which as a spot in the code where an identifier is
   /// followed by a parenthetical.
   /// </summary>
   class InvokeGroupItem : GroupItem
   {
      #region Private Fields

      private string identifier;
      private ParentheticGroup callParameters;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class InvokeGroupItem
      /// </summary>
      /// <param name="identifier">the identifier of the thing being invoked</param>
      /// <param name="callParameters">the parameters</param>
      public InvokeGroupItem(GroupItem identifier, GroupItem callParameters)
      {
         var token = ((TokenGroupItem)identifier).Token;
         this.identifier = ((IdentifierToken)token).Identifier;
         this.callParameters = (ParentheticGroup)callParameters;
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Compiles the item
      /// </summary>
      /// <param name="linkContext"></param>
      public override void Compile(LinkContext linkContext)
      {
         // the context needs to resolve this, since it knows what the identifier
         // resolves to
         linkContext.CompileInvocation(identifier, callParameters);
      }

      #endregion
   }
}
