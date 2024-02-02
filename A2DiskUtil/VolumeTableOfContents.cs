namespace A2DiskUtil
{
   public class VolumeTableOfContents
   {
      private Sector sector;

      public VolumeTableOfContents(Sector sector)
      {
         this.sector = sector;
      }

      public TrackSector FirstCatalogSector
      {
         get 
         {
            return new TrackSector(
               sector.ReadByte(1),
               sector.ReadByte(2)
               );
         }
      }
   }
}