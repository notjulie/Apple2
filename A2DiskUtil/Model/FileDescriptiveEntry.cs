﻿using System.Text;
using System.Xml.Linq;

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a file descriptive entry
   /// </summary>
   public class FileDescriptiveEntry
   {
      #region Types / Constants

      private const int TrackSectorOffset = 0x00;
      private const int Size = 34;

      #endregion

      #region Private Fields

      private byte[] data = new byte[Size];

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class FileDescriptiveEntry
      /// </summary>
      public FileDescriptiveEntry(A2FileName name)
      {
         // copy the name
         FileName = name;
      }

      /// <summary>
      /// Initializes a new instance of class FileDescriptiveEntry
      /// </summary>
      /// <param name="data"></param>
      public FileDescriptiveEntry(byte[] data)
      {
         // save the raw data
         Array.Copy(data, this.data, this.data.Length);
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets a value indicating whether this entry is actually associated with a file
      /// </summary>
      public bool IsFile
      {
         get
         {
            // a value of 0 is an unallocated file, a value of 255 is a
            // deleted file
            return data[0] > 0 && data[0] < 255;
         }
      }

      /// <summary>
      /// Gets or sets the filename
      /// </summary>
      public A2FileName FileName
      {
         get
         {
            // the filename is 30 bytes starting at offset 3
            byte[] filename = new byte[A2FileName.MaxLength];
            Array.Copy(data, 3, filename, 0, filename.Length);
            return new A2FileName(filename);
         }
         set
         {
            byte[] rawName = value.PadToFullLength().ToArray();
            Array.Copy(rawName, 0, data, 3, rawName.Length);
         }
      }

      /// <summary>
      /// Gets or sets the FileType
      /// </summary>
      public A2FileType FileType
      {
         get
         {
            return (A2FileType)(data[2] & 0x7F);
         }
         set
         {
            data[2] = (byte)((data[2] & 0x80) | (byte)value);
         }
      }

      /// <summary>
      /// Gets or sets the first sector of the track-sector list
      /// </summary>
      public TrackSector TrackSectorListStart
      {
         get
         {
            return new TrackSector(data[TrackSectorOffset], data[TrackSectorOffset + 1]);
         }
         set
         {
            data[TrackSectorOffset] = value.Track;
            data[TrackSectorOffset + 1] = value.Sector;
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Returns a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return FileName.ToString();
      }

      /// <summary>
      /// Returns our data as an array
      /// </summary>
      /// <returns></returns>
      public byte[] ToArray()
      {
         byte[] result = new byte[data.Length];
         Array.Copy(data, result, result.Length);
         return result;
      }

      #endregion
   }
}