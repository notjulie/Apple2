using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Link;

namespace Suna.Groups
{
   class InvokeGroupItem : GroupItem
   {
      #region Private Fields

      private GroupItem identifier;
      private GroupItem callParameters;

      #endregion

      #region Constructor

      public InvokeGroupItem(GroupItem identifier, GroupItem callParameters)
      {
         this.identifier = identifier;
         this.callParameters = callParameters;
      }

      #endregion

      #region Base Class Overrides

      public override void Compile(LinkContext linkContext)
      {
         throw new NotImplementedException();
      }

      #endregion
   }
}
