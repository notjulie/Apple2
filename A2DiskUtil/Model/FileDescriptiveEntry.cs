using System.Text;
using System.Xml.Linq;

namespace A2DiskUtil.Model
{
   public class FileDescriptiveEntry
   {
      #region Private Fields

      private byte[] data;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class FileDescriptiveEntry
      /// </summary>
      public FileDescriptiveEntry(A2FileName name)
      {
         // initialize blank
         data = new byte[Size];

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
         this.data = data;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the raw size of the entry
      /// </summary>
      public static int Size
      {
         get;
      } = 34;

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
            Array.Copy(data, 3, rawName, 0, rawName.Length);
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

      public TrackSector TrackSectorListStart
      {
         get
         {
            throw new NotImplementedException("FileDescriptiveEntry.TrackSectorListStart get");
         }
         set
         {
            throw new NotImplementedException("FileDescriptiveEntry.TrackSectorListStart set");
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

      #endregion
   }
}