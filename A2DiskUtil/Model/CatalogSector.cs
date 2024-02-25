using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a sector in the catalog
   /// </summary>
   public class CatalogSector
   {
      #region Private Fields

      private Sector sector;

      #endregion

      #region Constructor

      public CatalogSector(Sector sector)
      {
         this.sector = sector;
      }

      #endregion

      #region Public Properties

      public TrackSector NextSector
      {
         get
         {
            return new TrackSector(sector.ReadByte(0x01), sector.ReadByte(0x02));
         }
      }

      #endregion

      #region Public Methods

      public FileDescriptiveEntry[] GetFileDescriptiveEntries()
      {
         List<FileDescriptiveEntry> result = new List<FileDescriptiveEntry>();
         for (int i = 11; i < 255; i += 35)
         {
            result.Add(new FileDescriptiveEntry(sector.Read(i, 35)));
         }
         return result.ToArray();
      }

      /// <summary>
      /// Writes the entry to this sector if there's an empty spot; if
      /// there are no empty entries it returns false
      /// </summary>
      /// <param name="entry"></param>
      /// <returns></returns>
      public bool TryAddFile(FileDescriptiveEntry newEntry)
      {
         // look for an empty entry
         for (int i = 11; i < 255; i += 35)
         {
            var oldEntry = new FileDescriptiveEntry(sector.Read(i, 35));
            if (!oldEntry.IsFile)
            {
               sector.Write(i, newEntry.ToArray());
               return true;
            }
         }

         return false;
      }

      public bool TryRemoveFile(FileDescriptiveEntry file)
      {
         throw new NotImplementedException("CatalogSector.TryRemoveFile");
      }

      public byte[] ToArray()
      {
         return sector.ToArray();
      }

      #endregion
   }
}
