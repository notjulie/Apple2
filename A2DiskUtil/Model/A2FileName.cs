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
      private byte[] rawName;

      public A2FileName(byte[] rawName)
      {
         this.rawName = rawName;
      }


      public override string ToString()
      {
         StringBuilder sb = new StringBuilder();
         foreach (byte b in rawName)
            sb.Append((char)(b & 0x7f));
         return sb.ToString();
      }
   }
}
