using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   public class CatalogSector
   {
      private Sector sector;

      public CatalogSector(Sector sector)
      {
         this.sector = sector;
      }

      public TrackSector NextSector
      {
         get
         {
            return new TrackSector(sector.ReadByte(0x01), sector.ReadByte(0x02));
         }
      }

      public FileDescriptiveEntry[] GetFileDescriptiveEntries()
      {
         List<FileDescriptiveEntry> result = new List<FileDescriptiveEntry>();
         for (int i = 11; i < 255; i += 35)
         {
            result.Add(new FileDescriptiveEntry(sector.Read(i, 35)));
         }
         return result.ToArray();
      }
   }
}
