namespace A2DiskUtil.Model
{
   /// <summary>
   /// Representation of the VolumeTableOfContents sector
   /// </summary>
   public class VolumeTableOfContents
   {
      #region Private Fields

      private Sector sector;

      #endregion

      #region Constructor

      public VolumeTableOfContents(Sector sector)
      {
         this.sector = sector;
      }

      #endregion

      #region Public Properties

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

      #endregion

      #region Public Methods

      public TrackSector AllocateSector()
      {
         throw new NotImplementedException("VolumeTableOfContents.AllocateSector");
      }

      #endregion
   }
}