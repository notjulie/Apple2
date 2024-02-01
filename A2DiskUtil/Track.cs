namespace A2DiskUtil
{
   public class Track
   {
      private byte[] trackData = new byte[16 * 256];

      public Track(byte[] sourceData, int offset)
      {
         Array.Copy(sourceData, offset, trackData, 0, trackData.Length);
      }

      public Sector GetSector(int sector)
      {
         return new Sector(trackData, 256 * sector);
      }
   }
}