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
   public class CatalogSector(Sector sector)
   {
      #region Types / Constants

      private const int FirstFileEntryOffset = 11;

      #endregion

      #region Private Fields

      private readonly Sector sector = sector;

      #endregion
      #region Constructor

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
         List<FileDescriptiveEntry> result = [];
         for (int i = FirstFileEntryOffset; i < 255; i += FileDescriptiveEntry.Size)
         {
            result.Add(new FileDescriptiveEntry(sector.Read(i, FileDescriptiveEntry.Size)));
         }
         return [.. result];
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
         for (int i = FirstFileEntryOffset; i < 255; i += FileDescriptiveEntry.Size)
         {
            var oldEntry = new FileDescriptiveEntry(sector.Read(i, FileDescriptiveEntry.Size));
            if (!oldEntry.IsFile)
            {
               sector.Write(i, newEntry.ToArray());
               return true;
            }
         }

         return false;
      }

      /// <summary>
      /// Removes the given file from our list; returns false if not found
      /// </summary>
      /// <param name="file"></param>
      /// <returns></returns>
      public bool TryRemoveFile(FileDescriptiveEntry file)
      {
         var entries = GetFileDescriptiveEntries();
         for (int i=0; i<entries.Length; i++) 
         {
            if (entries[i] == file)
            {
               int offset = FirstFileEntryOffset + FileDescriptiveEntry.Size * i;
               for (int j = 0; j < FileDescriptiveEntry.Size; ++j)
                  sector.WriteByte(offset + j, 0);
               return true;
            }
         }

         return false;
      }

      public byte[] ToArray()
      {
         return sector.ToArray();
      }

      #endregion
   }
}
