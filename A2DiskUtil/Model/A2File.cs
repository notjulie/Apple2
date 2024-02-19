using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a file of my A2 format... this is simply:
   ///   [two byte start address]
   ///   [N byte binary image]
   /// </summary>
   public class A2File
   {
      #region Private Methods

      private byte[] rawContents;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class A2File from the given filename
      /// </summary>
      /// <param name="filename"></param>
      public A2File(string filename)
      {
         rawContents = File.ReadAllBytes(filename);
         A2FileName = new A2FileName(Path.GetFileNameWithoutExtension(filename));
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the native filename
      /// </summary>
      public A2FileName A2FileName
      {
         get;
         private set;
      }

      public byte[] BinaryImage
      {
         get
         {
            throw new NotImplementedException("A2File.BinaryImage");
         }
      }

      public UInt16 StartAddress
      {
         get
         {
            throw new NotImplementedException("A2File.StartAddress");
         }
      }

      #endregion
   }
}
