using System;
using System.Collections.Generic;
using System.Linq;
using System.Printing.IndexedProperties;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   public class A2FileName
   {
      #region Private Fields

      private byte[] rawName;

      #endregion

      #region Constructor

      public A2FileName(byte[] rawName)
      {
         this.rawName = rawName;
      }

      public A2FileName(string stringName)
      {
         List<byte> bytes = new List<byte>();

         foreach (char c in stringName)
         {
            // convert to byte
            byte b = (byte)c;

            // convert lower case to upper
            if (b >= 0x61 && b <= 0x7A)
               b -= 0x20;

            // translate to the weird Apple2 character set
            if (b >= 0x20 && b <= 0x5F)
            {
               bytes.Add((byte)((b & 0x3F) | 0x80));
            }
            else
            {
               throw new ArgumentException("Invalid characters in A2FileName string");
            }
         }

         this.rawName = bytes.ToArray();
      }

      #endregion

      #region Base Class Overrides

      public override string ToString()
      {
         StringBuilder sb = new StringBuilder();
         foreach (byte b in rawName)
            sb.Append((char)(b & 0x7f));
         return sb.ToString();
      }

      #endregion
   }
}
