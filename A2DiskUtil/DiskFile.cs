using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil
{
   /// <summary>
   /// Representation of a disk image
   /// </summary>
   class DiskFile
   {
      private byte[] fileData;

      /// <summary>
      /// Initializes a new disk image from a file
      /// </summary>
      /// <param name="filename"></param>
      public DiskFile(string filename)
      {
         fileData = File.ReadAllBytes(filename);
      }
   }
}
