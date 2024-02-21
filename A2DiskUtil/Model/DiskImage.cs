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

         // read it
         VolumeTableOfContents = new VolumeTableOfContents(GetTrack(0x11).GetSector(0x0));
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

      #region Public Properties

      public VolumeTableOfContents VolumeTableOfContents
      {
         get;
         private set;
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
      public Track GetTrack(int track)
      {
         return new Track(fileData, track * 16 * 256);
      }

      public void DeleteIfExists(A2FileName fileName)
      {
         // look for the file's catalog entry
         foreach (var entry in GetCatalog())
         {
            if (entry.FileName == fileName)
            {
               Delete(entry);
               break;
            }
         }
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
         DeleteIfExists(name);

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

      private void AddFileEntry(FileDescriptiveEntry entry)
      {
         throw new NotImplementedException("DiskImage.AddFileEntry");
      }

      private TrackSector AllocateSector()
      {
         throw new NotImplementedException("DiskImage.AllocateSector");
      }

      private void Delete(FileDescriptiveEntry file)
      {
         throw new NotImplementedException("DiskImage.Delete");
      }

      /// <summary>
      /// Allocates sectors and writes the given data to them, returning
      /// the list of sectors to which the data was written
      /// </summary>
      /// <param name="data"></param>
      /// <returns></returns>
      private TrackSector[] WriteDataToAvailableSectors(byte[] data)
      {
         List<TrackSector> sectors = new List<TrackSector>();

         for (int offset = 0; offset < data.Length; offset += Sector.Size)
         {
            int length = data.Length - offset;
            if (length > Sector.Size)
               length = Sector.Size;

            byte[] sectorData = new byte[length];
            Array.Copy(data, offset, sectorData, 0, sectorData.Length);

            TrackSector sector = AllocateSector();
            WriteSector(sector, sectorData);
            sectors.Add(sector);
         }

         return sectors.ToArray();
      }

      private void WriteSector(TrackSector sector, byte[] sectorData)
      {
         throw new NotImplementedException("DiskImage.WriteSector");
      }

      private TrackSector WriteTrackSectorList(TrackSector[] trackSectorList)
      {
         throw new NotImplementedException("DiskImage.WriteTrackSectorList");
      }

      #endregion
   }
}
