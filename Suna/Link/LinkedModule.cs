using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Link
{
   class LinkedModule
   {
      #region Private Fields

      private List<byte> code = new List<byte>();

      #endregion

      #region Public Methods

      public void AppendCode(byte[] code)
      {
         this.code.AddRange(code);
      }

      #endregion
   }
}
