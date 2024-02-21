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

      /// <summary>
      /// Initializes a new instance of class VolumeTableOfContents
      /// </summary>
      /// <param name="sector"></param>
      public VolumeTableOfContents(Sector sector)
      {
         this.sector = sector;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the address of the first sector of the catalog
      /// </summary>
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

      /// <summary>
      /// Gets the number of tracks
      /// </summary>
      public byte TrackCount
      {
         get
         {
            return sector.ReadByte(0x34);
         }
      }

      /// <summary>
      /// Gets the number of sectors per track
      /// </summary>
      public byte SectorsPerTrack
      {
         get
         {
            return sector.ReadByte(0x35);
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Allocates a sector
      /// </summary>
      /// <returns></returns>
      public TrackSector AllocateSector()
      {
         // track 0 is not used according to what I read
         for (byte track = 1; track < TrackCount;  track++) 
         {
            for (byte sector = 0; sector < SectorsPerTrack; ++sector)
            {
               TrackSector trackSector = new TrackSector(track, sector);
               if (!IsAllocated(trackSector))
               {
                  Allocate(trackSector);
                  return trackSector;
               }
            }
         }

         throw new Exception("TrackSector.AllocateSector: disk full");
      }

      #endregion

      #region Private Methods

      /// <summary>
      /// returns a value indicating whether the given sector is allocated
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      private bool IsAllocated(TrackSector trackSector)
      {
         byte byteOffset, bitMask;
         GetBitLocation(trackSector, out byteOffset, out bitMask);
         return (sector.ReadByte(byteOffset) & bitMask) != 0;
      }

      /// <summary>
      /// marks the given sector as allocated
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      private void Allocate(TrackSector trackSector)
      {
         byte byteOffset, bitMask;
         GetBitLocation(trackSector, out byteOffset, out bitMask);
         byte b = (byte)(sector.ReadByte(byteOffset) | bitMask);
         sector.WriteByte(byteOffset, b);
      }

      private void GetBitLocation(TrackSector trackSector, out byte byteOffset, out byte bitMask)
      {
         // https://gswv.apple2.org.za/a2zine/faqs/Csa2DOSMM.html#019
         byteOffset = (byte)(0x38 + 4 * trackSector.Track);
         if (trackSector.Sector < 8)
         {
            byteOffset += 1;
            bitMask = (byte)(1 << trackSector.Sector);
         }
         else
         {
            bitMask = (byte)(1 << trackSector.Sector - 8);
         }
      }

      #endregion
   }
}