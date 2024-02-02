using System.Text;

namespace A2DiskUtil
{
   public class FileDescriptiveEntry
   {
      private byte[] data;

      public FileDescriptiveEntry(byte[] data)
      {
         this.data = data;

         StringBuilder sb = new StringBuilder();
         for (int i=3; i<=32; ++i)
            sb.Append((char)(data[i] & 0x7f));
         FileName = sb.ToString();
      }

      public string FileName
      {
         get;
         private set;
      }
   }
}