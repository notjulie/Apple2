using System.Text;

namespace A2DiskUtil
{
   public class FileDescriptiveEntry
   {
      #region Private Fields

      private byte[] data;

      #endregion

      #region Constructor

      public FileDescriptiveEntry(byte[] data)
      {
         this.data = data;

         StringBuilder sb = new StringBuilder();
         for (int i = 3; i <= 32; ++i)
            sb.Append((char)(data[i] & 0x7f));
         FileName = sb.ToString();
      }

      #endregion

      #region Public Properties

      public string FileName
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
         return FileName;
      }

      #endregion
   }
}