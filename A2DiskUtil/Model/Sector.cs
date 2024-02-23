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

      public Sector(byte[] sourceData, int offset)
      {
         Array.Copy(sourceData, offset, sectorData, 0, sectorData.Length);
      }

      #endregion

      #region Public Methods

      public byte[] Read(int offset, int length)
      {
         byte[] result = new byte[length];
         Array.Copy(sectorData, offset, result, 0, length);
         return result;
      }

      public byte ReadByte(int offset)
      {
         return Read(offset, 1)[0];
      }

      public void WriteByte(int offset, byte value)
      {
         sectorData[offset] = value;
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