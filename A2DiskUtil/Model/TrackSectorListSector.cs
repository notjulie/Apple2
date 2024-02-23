using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   public class TrackSectorListSector
   {
      public TrackSectorListSector()
      {
         throw new NotImplementedException("TrackSectorListSector.TrackSectorListSector");
      }

      public TrackSector NextSector
      {
         get
         {
            throw new NotImplementedException("TrackSectorListSector.NextSector get");
         }
         set
         {
            throw new NotImplementedException("TrackSectorListSector.NextSector set");
         }
      }

      public void AppendSector(TrackSector trackSector)
      {
         throw new NotImplementedException("TrackSectorListSector.AppendSector");
      }

      public bool TryAppendSector(TrackSector trackSector)
      {
         throw new NotImplementedException("TrackSectorListSector.AppendSector");
      }

      public byte[] ToArray()
      {
         throw new NotImplementedException("TrackSectorListSector.ToArray");
      }
   }
}
