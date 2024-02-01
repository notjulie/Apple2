namespace A2DiskUtil
{
   public class Sector
   {
      private byte[] sectorData = new byte[256];

      public Sector(byte[] sourceData, int offset) 
      {
         Array.Copy(sourceData, offset, sectorData, 0, sectorData.Length);
      }
   }
}