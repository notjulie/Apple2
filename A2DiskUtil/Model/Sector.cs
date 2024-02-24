namespace A2DiskUtil.Model
{
   /// <summary>
   /// Representation of a disk sector
   /// </summary>
   public class Sector
   {
      #region Types / Constants

      public const int Size = 256;

      #endregion

      #region Private Fields

      private byte[] sectorData = new byte[Size];

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class Sector
      /// </summary>
      /// <param name="sourceData"></param>
      /// <param name="offset"></param>
      public Sector(byte[] sourceData, int offset)
      {
         Array.Copy(sourceData, offset, sectorData, 0, sectorData.Length);
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Reads a section of the sector
      /// </summary>
      /// <param name="offset"></param>
      /// <param name="length"></param>
      /// <returns></returns>
      public byte[] Read(int offset, int length)
      {
         byte[] result = new byte[length];
         Array.Copy(sectorData, offset, result, 0, length);
         return result;
      }

      /// <summary>
      /// Reads a byte at a given offset
      /// </summary>
      /// <param name="offset"></param>
      /// <returns></returns>
      public byte ReadByte(int offset)
      {
         return Read(offset, 1)[0];
      }

      /// <summary>
      /// Writes a byte at a given offset
      /// </summary>
      /// <param name="offset"></param>
      /// <param name="value"></param>
      public void WriteByte(int offset, byte value)
      {
         sectorData[offset] = value;
      }

      /// <summary>
      /// Writes a section of the sector
      /// </summary>
      /// <param name="offset"></param>
      /// <param name="data"></param>
      public void Write(int offset, byte[] data)
      {
         Array.Copy(data, 0, sectorData, offset, data.Length);
      }

      /// <summary>
      /// Returns the sector's data as a byte array
      /// </summary>
      /// <returns>the sector's data as a byte array</returns>
      public byte[] ToArray()
      {
         byte[] result = new byte[sectorData.Length];
         Array.Copy(sectorData, result, sectorData.Length);
         return result;
      }

      #endregion
   }
}