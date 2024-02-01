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
         VolumeTableOfContents = new VolumeTableOfContents(GetTrack(0x11).GetSector(0x0));
      }

      public VolumeTableOfContents VolumeTableOfContents
      {
         get;
         private set;
      }

      public Track GetTrack(int track)
      {
         return new Track(fileData, track * 16 * 256);
      }
   }
}
