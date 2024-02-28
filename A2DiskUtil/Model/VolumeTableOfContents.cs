namespace A2DiskUtil.Model
{
   /// <summary>
   /// Representation of the VolumeTableOfContents sector
   /// </summary>
   /// <remarks>
   /// Initializes a new instance of class VolumeTableOfContents
   /// </remarks>
   /// <param name="sector"></param>
   public class VolumeTableOfContents(Sector _sector)
   {
      #region Types/Constants

      /// <summary>
      /// offset and mask of a bit in our allocation map
      /// </summary>
      private struct BitLocation
      {
         public byte offset;
         public byte mask;
      }

      #endregion

      #region Private Fields

      private readonly Sector sector = _sector;

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
      public TrackSector AllocateNewSector()
      {
         // track 0 is not used according to what I read
         for (byte track = 1; track < TrackCount;  track++) 
         {
            for (byte sector = 0; sector < SectorsPerTrack; ++sector)
            {
               TrackSector trackSector = new(track, sector);
               if (!IsAllocated(trackSector))
               {
                  AllocateSector(trackSector);
                  return trackSector;
               }
            }
         }

         throw new Exception("TrackSector.AllocateSector: disk full");
      }

      /// <summary>
      /// Marks the sector as free
      /// </summary>
      /// <param name="sector"></param>
      public void DeallocateSector(TrackSector trackSector)
      {
         // set its available bit
         var bitLocation = GetAllocationBitLocation(trackSector);
         byte b = (byte)(sector.ReadByte(bitLocation.offset) | bitLocation.mask);
         sector.WriteByte(bitLocation.offset, b);
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
         var bitLocation = GetAllocationBitLocation(trackSector);
         return (sector.ReadByte(bitLocation.offset) & bitLocation.mask) == 0;
      }

      /// <summary>
      /// marks the given sector as allocated
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      private void AllocateSector(TrackSector trackSector)
      {
         // clear its available bit
         var bitLocation = GetAllocationBitLocation(trackSector);
         byte b = (byte)(sector.ReadByte(bitLocation.offset) & ~bitLocation.mask);
         sector.WriteByte(bitLocation.offset, b);
      }

      /// <summary>
      /// Gets the byte offset and bit mask of the sector free bit for the
      /// given sector
      /// </summary>
      /// <param name="trackSector">the sector</param>
      private static BitLocation GetAllocationBitLocation(TrackSector trackSector)
      {
         // https://gswv.apple2.org.za/a2zine/faqs/Csa2DOSMM.html#019

         // start with the offset of the 4 byte region for the track
         BitLocation result = new()
         {
            offset = (byte)(0x38 + 4 * trackSector.Track)
         };

         // adjust for the odd positioning of the bits within the 4 bytes
         if (trackSector.Sector < 8)
         {
            result.offset += 1;
            result.mask = (byte)(1 << trackSector.Sector);
         }
         else
         {
            result.mask = (byte)(1 << trackSector.Sector - 8);
         }

         // done
         return result;
      }

      #endregion
   }
}