namespace A2DiskUtil.Model
{
   public class TrackSector
   {
      public TrackSector(byte track, byte sector)
      {
         Track = track;
         Sector = sector;
      }

      public byte Track { get; private set; }
      public byte Sector { get; private set; }

      static public TrackSector Null
      {
         get;
      } = new TrackSector(0, 0);
   }
}