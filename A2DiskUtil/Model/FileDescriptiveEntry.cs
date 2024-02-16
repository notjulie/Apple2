using System.Text;

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
      /// <param name="data"></param>
      public FileDescriptiveEntry(byte[] data)
      {
         // save the raw data
         this.data = data;

         // the filename is 30 bytes starting at offset 3
         byte[] filename = new byte[30];
         Array.Copy(data, 3, filename, 0, filename.Length);
         FileName = new A2FileName(filename);
      }

      #endregion

      #region Public Properties

      public A2FileName FileName
      {
         get;
         private set;
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