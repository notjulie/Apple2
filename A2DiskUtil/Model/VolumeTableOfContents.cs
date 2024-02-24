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

      /// <summary>
      /// Returns the table of contents as a byte array
      /// </summary>
      /// <returns>the table of contents as a byte array</returns>
      public byte[] ToArray()
      {
         return sector.ToArray();
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
         // a bit that is set indicates that the sector is available
         byte byteOffset, bitMask;
         GetAllocationBitLocation(trackSector, out byteOffset, out bitMask);
         return (sector.ReadByte(byteOffset) & bitMask) == 0;
      }

      /// <summary>
      /// marks the given sector as allocated
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      private void Allocate(TrackSector trackSector)
      {
         // clear its available bit
         byte byteOffset, bitMask;
         GetAllocationBitLocation(trackSector, out byteOffset, out bitMask);
         byte b = (byte)(sector.ReadByte(byteOffset) & ~bitMask);
         sector.WriteByte(byteOffset, b);
      }

      /// <summary>
      /// Gets the byte offset and bit mask of the sector free bit for the
      /// given sector
      /// </summary>
      /// <param name="trackSector">the sector</param>
      /// <param name="byteOffset">the returned byteOffset</param>
      /// <param name="bitMask">the returned bitmask</param>
      private void GetAllocationBitLocation(TrackSector trackSector, out byte byteOffset, out byte bitMask)
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