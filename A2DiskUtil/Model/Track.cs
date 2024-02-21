// 
// Author: Randy Rasmussen
// Copyright: None, use as you will
// Warranties: None, use at your own risk
//

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a snapshot of a track
   /// </summary>
   public class Track
   {
      #region Types / Constants

      public const int TrackSize = 16 * Sector.Size;

      #endregion

      #region Private Fields

      private byte[] trackData = new byte[TrackSize];

      #endregion

      #region Constructor

      public Track(byte[] sourceData, int offset)
      {
         Array.Copy(sourceData, offset, trackData, 0, trackData.Length);
      }

      #endregion

      #region Public Methods

      public Sector GetSector(int sector)
      {
         return new Sector(trackData, 256 * sector);
      }

      #endregion
   }
}