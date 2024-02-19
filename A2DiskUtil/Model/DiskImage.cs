﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   /// <summary>
   /// Representation of a disk image
   /// </summary>
   public class DiskImage
   {
      #region Private Fields

      private byte[] fileData;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new disk image from a file
      /// </summary>
      /// <param name="filename"></param>
      public DiskImage(string filename)
      {
         fileData = File.ReadAllBytes(filename);
         VolumeTableOfContents = new VolumeTableOfContents(GetTrack(0x11).GetSector(0x0));

         //GetCatalog();
      }

      #endregion

      #region Public Properties

      public VolumeTableOfContents VolumeTableOfContents
      {
         get;
         private set;
      }

      #endregion

      #region Public Methods

      public DiskImage Clone()
      {
         throw new NotImplementedException("DiskImage.Clone");
      }

      public FileDescriptiveEntry[] GetCatalog()
      {
         List<FileDescriptiveEntry> result = new List<FileDescriptiveEntry>();

         TrackSector trackSector = VolumeTableOfContents.FirstCatalogSector;
         while (trackSector.Track != 0 && trackSector.Sector != 0)
         {
            Sector sector = GetSector(trackSector);
            for (int i = 11; i < 255; i += 35)
            {
               result.Add(new FileDescriptiveEntry(sector.Read(i, 35)));
            }

            trackSector = new TrackSector(
               sector.ReadByte(1),
               sector.ReadByte(2)
               );
         }

         return result.ToArray();
      }

      public Sector GetSector(TrackSector trackSector)
      {
         return GetTrack(trackSector.Track).GetSector(trackSector.Sector);
      }

      public Track GetTrack(int track)
      {
         return new Track(fileData, track * 16 * 256);
      }

      public void WriteFile(A2File file)
      {
         throw new NotImplementedException("DiskImage.WriteFile");
      }

      #endregion
   }
}
