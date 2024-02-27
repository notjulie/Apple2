using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.ExceptionServices;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Represents a sector of a track-sector list
   /// </summary>
   public class TrackSectorListSector
   {
      #region Types / Constants

      private const int NextTrackSectorOffset = 0x01;
      private const int SectorListOffset = 0x0C;

      #endregion

      #region Private Fields

      private byte[] sectorData = new byte[Sector.Size];

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class TrackSectorListSector
      /// </summary>
      public TrackSectorListSector()
      {
         // all good, nothing but zeroes is perfect
      }

      /// <summary>
      /// Initializes a new instance of class TrackSectorListSector from the
      /// given sector
      /// </summary>
      /// <param name="sector"></param>
      public TrackSectorListSector(Sector sector)
      {
         sectorData = sector.ToArray();
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets or sets the next sector in the list
      /// </summary>
      public TrackSector NextSector
      {
         get
         {
            return new TrackSector(sectorData[NextTrackSectorOffset], sectorData[NextTrackSectorOffset + 1]);
         }
         set
         {
            sectorData[NextTrackSectorOffset] = value.Track;
            sectorData[NextTrackSectorOffset + 1] = value.Sector;
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Appends the sector to the sector list; throws an exception
      /// if the list is full
      /// </summary>
      /// <param name="trackSector"></param>
      public void AppendSector(TrackSector trackSector)
      {
         if (!TryAppendSector(trackSector)) 
         {
            throw new Exception("Track-sector list is full");
         }
      }

      /// <summary>
      /// Returns the list of sectors
      /// </summary>
      /// <returns></returns>
      public TrackSector[] GetSectors()
      {
         List<TrackSector> result = new List<TrackSector>();

         for (int i = SectorListOffset; i < sectorData.Length - 1; ++i)
         {
            TrackSector sector = new TrackSector(sectorData[i], sectorData[i + 1]);
            if (sector.IsNull)
               break;
            result.Add(sector);
         }

         return result.ToArray();
      }

      /// <summary>
      /// Appends the sector to the sector list; returns false
      /// if the list is full
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      public bool TryAppendSector(TrackSector trackSector)
      {
         for (int i = SectorListOffset; i < sectorData.Length - 2; i+= 2)
         {
            if (sectorData[i] == 0 && sectorData[i+1] == 0)
            {
               sectorData[i] = trackSector.Track;
               sectorData[i + 1] = trackSector.Sector;
               return true;
            }
         }

         return false;
      }

      /// <summary>
      /// Return the sector as a byte array
      /// </summary>
      /// <returns></returns>
      public byte[] ToArray()
      {
         byte[] result = new byte[Sector.Size];
         Array.Copy(sectorData, result, sectorData.Length);
         return result;
      }

      #endregion
   }
}
