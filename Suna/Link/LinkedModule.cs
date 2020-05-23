using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Link
{
   /// <summary>
   /// Represents the final result of a compile/link operation
   /// </summary>
   public class LinkedModule
   {
      #region Private Fields

      private List<byte> code = new List<byte>();

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the base address
      /// </summary>
      public UInt16 BaseAddress
      {
         get;
      } = 0x6000;

      #endregion

      #region Public Methods

      /// <summary>
      /// Appends the given chunk of machine code to the output
      /// </summary>
      /// <param name="code"></param>
      public void AppendCode(byte[] code)
      {
         this.code.AddRange(code);
      }

      /// <summary>
      /// Returns the image
      /// </summary>
      /// <returns></returns>
      public byte[] GetImage()
      {
         return code.ToArray();
      }

      #endregion
   }
}
