// 
// Author: Randy Rasmussen
// Copyright: None, use as you will
// Warranties: None, use at your own risk
//

using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.DirectoryServices;
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
      #region Types / Constants

      private const int TableOfContentsTrack = 0x11;
      private const int TableOfContentsSector = 0;

      private struct CatalogSectorAndLocation
      {
         public TrackSector Location;
         public CatalogSector Sector;
      }

      #endregion

      #region Private Fields

      private byte[] fileData;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new disk image from a byte array
      /// </summary>
      /// <param name="fileData"></param>
      public DiskImage(byte[] fileData)
      {
         // make a copy of the contents
         this.fileData = new byte[fileData.Length];
         Array.Copy(fileData, this.fileData, fileData.Length);
      }

      /// <summary>
      /// Initializes a new disk image from a file
      /// </summary>
      /// <param name="filename"></param>
      public DiskImage(string filename)
         :this(File.ReadAllBytes(filename))
      {
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Creates a copy of this instance
      /// </summary>
      /// <returns></returns>
      public DiskImage Clone()
      {
         return new DiskImage(this.fileData);
      }

      /// <summary>
      /// Reads the list of all FileDescriptiveEntry blocks from the disk, active or
      /// inactive
      /// </summary>
      /// <returns></returns>
      public FileDescriptiveEntry[] GetCatalog()
      {
         List<FileDescriptiveEntry> result = new List<FileDescriptiveEntry>();
         foreach (var entry in ReadCatalogSectors())
         {
            result.AddRange(entry.Sector.GetFileDescriptiveEntries());
         }
         return result.ToArray();
      }

      /// <summary>
      /// Gets the contents of the given sector
      /// </summary>
      /// <param name="trackSector"></param>
      /// <returns></returns>
      public Sector GetSector(TrackSector trackSector)
      {
         return GetTrack(trackSector.Track).GetSector(trackSector.Sector);
      }

      /// <summary>
      /// Gets the contents of the given tract
      /// </summary>
      /// <param name="track"></param>
      /// <returns></returns>
      public Track GetTrack(byte track)
      {
         return new Track(fileData, GetTrackOffset(track));
      }

      /// <summary>
      /// Returns a value indicating whether the file exists
      /// </summary>
      /// <param name="fileName"></param>
      /// <returns></returns>
      public bool FileExists(A2FileName fileName)
      {
         return GetFileEntry(fileName) != null;
      }

      /// <summary>
      /// Deletes the given file
      /// </summary>
      /// <param name="fileName"></param>
      public void DeleteFile(A2FileName fileName)
      {
         Delete(GetFileEntry(fileName));
      }

      public void SaveAs(string fileName)
      {
         File.WriteAllBytes(fileName, this.fileData);
      }

      /// <summary>
      /// Writes a A2File to the disk
      /// </summary>
      /// <param name="file"></param>
      /// <exception cref="NotImplementedException"></exception>
      public void WriteFile(A2File file)
      {
         BSAVE(file.A2FileName, file.StartAddress, file.BinaryImage);
      }

      #endregion

      #region DOS-like commands

      /// <summary>
      /// Saves a file similarly to BSAVE; it's a binary file with a start address
      /// </summary>
      /// <param name="name"></param>
      /// <param name="startAddress"></param>
      /// <param name="binaryImage"></param>
      /// <exception cref="NotImplementedException"></exception>
      public void BSAVE(A2FileName name, UInt16 startAddress, byte[] binaryImage)
      {
         // delete any such existing file
         if (FileExists(name))
            DeleteFile(name);

         // create a catalog entry
         FileDescriptiveEntry entry = new FileDescriptiveEntry(name);
         entry.FileType = A2FileType.Binary;

         // build the file contents... binary format goes:
         //   address
         //   length
         //   data
         List<byte> fileContents = new List<byte>();
         fileContents.Add((byte)(startAddress >> 8));
         fileContents.Add((byte)(startAddress & 0xFF));
         fileContents.Add((byte)(binaryImage.Length >> 8));
         fileContents.Add((byte)(binaryImage.Length & 0xFF));
         fileContents.AddRange(binaryImage);

         // write the file contents to a list of sectors
         TrackSector[] trackSectorList = WriteDataToAvailableSectors(fileContents.ToArray());

         // write the track/sector list
         entry.TrackSectorListStart = WriteTrackSectorList(trackSectorList);

         // and write the entry
         AddFileEntry(entry);
      }

      #endregion

      #region Private Methods

      /// <summary>
      /// Adds the given file entry to the current catalog
      /// </summary>
      /// <param name="entry"></param>
      private void AddFileEntry(FileDescriptiveEntry entry)
      {
         // go through the catalog sectors until we find one that
         // we can add the entry to
         foreach (var catalogSector in ReadCatalogSectors())
         {
            if (catalogSector.Sector.TryAddFile(entry))
            {
               WriteSector(catalogSector.Location, catalogSector.Sector.ToArray());
               return;
            }
         }

         throw new Exception("DiskImage.AddFileEntry: catalog is full");
      }

      /// <summary>
      /// Deletes the given file
      /// </summary>
      /// <param name="file"></param>
      private void Delete(FileDescriptiveEntry file)
      {
         // read the table of contents
         VolumeTableOfContents tableOfContents = ReadTableOfContents();

         // get the file's track-sector list
         TrackSector[] trackSectorList = GetTrackSectorList(file);

         // deallocate all of the file's sectors
         foreach (TrackSector trackSectorListSector in trackSectorList)
         {
            // deallocate everything referenced by this sector
            TrackSectorListSector sector = ReadTrackSectorListSector(trackSectorListSector);
            foreach (TrackSector trackSector in sector.GetSectors())
               tableOfContents.DeallocateSector(trackSector);

            // deallocate this sector
            tableOfContents.DeallocateSector(trackSectorListSector);
         }

         // rewrite the table of contents
         WriteTableOfContents(tableOfContents);

         // remove the file from the catalog
         foreach (var catalogSector in ReadCatalogSectors())
         {
            if (catalogSector.Sector.TryRemoveFile(file))
            {
               WriteSector(catalogSector.Location, catalogSector.Sector.ToArray());
               return;
            }
         }

         throw new Exception("DiskImage.Delete: unable to delete from catalog");
      }

      /// <summary>
      /// Gets the FileDescriptiveEntry associated with the given filename;
      /// returns null if the file doesn't exist
      /// </summary>
      /// <param name="fileName"></param>
      /// <returns></returns>
      private FileDescriptiveEntry GetFileEntry(A2FileName fileName)
      {
         // look for the file's catalog entry
         foreach (var entry in GetCatalog())
         {
            if (entry.FileName == fileName)
            {
               return entry;
            }
         }

         return null;
      }

      private int GetSectorOffset(TrackSector sector)
      {
         return GetTrackOffset(sector.Track) + sector.Sector * Sector.Size;
      }

      private int GetTrackOffset(byte track)
      {
         return track * Track.TrackSize;
      }

      private TrackSector[] GetTrackSectorList(FileDescriptiveEntry file)
      {
         throw new NotImplementedException("DiskImage.GetTrackSectorList");
      }

      /// <summary>
      /// Reads the list of all CatalogEntry sectors from the disk
      /// </summary>
      /// <returns></returns>
      private CatalogSectorAndLocation[] ReadCatalogSectors()
      {
         List<CatalogSectorAndLocation> result = new List<CatalogSectorAndLocation>();
         TrackSector trackSector = ReadTableOfContents().FirstCatalogSector;
         while (trackSector.Track != 0 && trackSector.Sector != 0)
         {
            // add this sector to the result
            Sector sector = GetSector(trackSector);
            CatalogSectorAndLocation entry = new CatalogSectorAndLocation();
            entry.Location = trackSector;
            entry.Sector = new CatalogSector(sector);
            result.Add(entry);

            // next sector
            trackSector = entry.Sector.NextSector;
         }

         return result.ToArray();
      }

      private VolumeTableOfContents ReadTableOfContents()
      {
         return new VolumeTableOfContents(GetTrack(TableOfContentsTrack).GetSector(TableOfContentsSector));
      }

      private TrackSectorListSector ReadTrackSectorListSector(TrackSector trackSector)
      {
         throw new NotImplementedException("DiskImage.ReadTrackSectorListSector");
      }

      /// <summary>
      /// Writes the contents of the volume table of contents sector
      /// </summary>
      /// <param name="tableOfContents"></param>
      private void WriteTableOfContents(VolumeTableOfContents tableOfContents)
      {
         WriteSector(new TrackSector(TableOfContentsTrack, TableOfContentsSector), tableOfContents.ToArray());
      }

      /// <summary>
      /// Allocates sectors and writes the given data to them, returning
      /// the list of sectors to which the data was written
      /// </summary>
      /// <param name="data"></param>
      /// <returns></returns>
      private TrackSector[] WriteDataToAvailableSectors(byte[] data)
      {
         // read the table of contents
         VolumeTableOfContents tableOfContents = ReadTableOfContents();

         // allocate sectors and write to them
         List<TrackSector> sectors = new List<TrackSector>();
         for (int offset = 0; offset < data.Length; offset += Sector.Size)
         {
            int length = data.Length - offset;
            if (length > Sector.Size)
               length = Sector.Size;

            byte[] sectorData = new byte[length];
            Array.Copy(data, offset, sectorData, 0, sectorData.Length);

            TrackSector sector = tableOfContents.AllocateSector();
            WriteSector(sector, sectorData);
            sectors.Add(sector);
         }

         // write the table of contents
         WriteTableOfContents(tableOfContents);

         // return the sectors that we wrote to
         return sectors.ToArray();
      }

      /// <summary>
      /// Writes data to the given sector
      /// </summary>
      /// <param name="sector"></param>
      /// <param name="sectorData"></param>
      private void WriteSector(TrackSector sector, byte[] sectorData)
      {
         int offset = GetSectorOffset(sector);
         Array.Copy(sectorData, 0, this.fileData, offset, sectorData.Length);
      }

      /// <summary>
      /// Writes the track-sector list and returns the address of the first
      /// sector
      /// </summary>
      /// <param name="trackSectorList"></param>
      /// <returns></returns>
      private TrackSector WriteTrackSectorList(TrackSector[] trackSectorList)
      {
         // read the table of contents
         VolumeTableOfContents tableOfContents = ReadTableOfContents();

         // create the track-sector list
         List<TrackSectorListSector> sectors = new List<TrackSectorListSector>();
         TrackSectorListSector currentSector = new TrackSectorListSector();
         sectors.Add(currentSector);
         foreach (var sector in trackSectorList)
         {
            // append to the current sector if we can, else start another
            if (!currentSector.TryAppendSector(sector))
            {
               currentSector = new TrackSectorListSector();
               sectors.Add(currentSector);
               currentSector.AppendSector(sector);
            }
         }

         // write them all... back to front so that we can make a nice little
         // linked list of them
         TrackSector result = TrackSector.Null;
         for (int i=sectors.Count-1; i>=0; i--)
         {
            sectors[i].NextSector = result;
            result = tableOfContents.AllocateSector();
            WriteSector(result, sectors[i].ToArray());
         }

         // update the table of contents
         WriteTableOfContents(tableOfContents);

         // return the address of the first sector
         return result;
      }

      #endregion
   }
}
