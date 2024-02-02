namespace A2DiskUtil
{
   public class TrackSector
   {
      public TrackSector(byte track, byte sector)
      {
         this.Track = track;
         this.Sector = sector;
      }

      public byte Track { get; private set; }
      public byte Sector { get; private set; }
   }
}