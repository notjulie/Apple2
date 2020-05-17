using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Groups
{
   class InvokeGroup : Group
   {
      private GroupItem identifier;
      private GroupItem callParameters;

      public InvokeGroup(GroupItem identifier, GroupItem callParameters)
      {
         this.identifier = identifier;
         this.callParameters = callParameters;
      }
   }
}
