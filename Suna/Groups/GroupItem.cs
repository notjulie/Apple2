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
   /// Represents an item in our grouped view of the code; this is just my way
   /// of dealing with the hierachical nature of code.  A GroupItem can be:
   ///    - a token
   ///    - a group of items bounded by brackets or parentheses
   ///    - other things as I see fit
   /// </summary>
   class GroupItem
   {
      #region Types / Constants

      private enum Error 
      {
         InvalidGroupForCompile
      }

      #endregion

      #region Constructor

      public GroupItem()
      {
      }

      #endregion

      #region Public Methods

      virtual public void Compile(LinkContext linkContext)
      {
         throw new CompileException(Error.InvalidGroupForCompile);
      }

      #endregion
   }
}
