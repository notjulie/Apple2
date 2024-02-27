namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a sector address as (track #, sector #)
   /// </summary>
   public class TrackSector
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class TrackSector
      /// </summary>
      /// <param name="track"></param>
      /// <param name="sector"></param>
      public TrackSector(byte track, byte sector)
      {
         Track = track;
         Sector = sector;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets a value indicating whether this is a null sector address
      /// </summary>
      public bool IsNull
      {
         get
         {
            return Track==0 && Sector==0;
         }
      }

      /// <summary>
      /// Gets the track number
      /// </summary>
      public byte Track
      {
         get;
         private set;
      }

      /// <summary>
      /// Gets the sector number
      /// </summary>
      public byte Sector
      {
         get;
         private set;
      }

      /// <summary>
      /// Returns a null instance
      /// </summary>
      static public TrackSector Null
      {
         get;
      } = new TrackSector(0, 0);

      #endregion
   }
}