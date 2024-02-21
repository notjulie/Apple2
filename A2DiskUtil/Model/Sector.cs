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

      #endregion
   }
}