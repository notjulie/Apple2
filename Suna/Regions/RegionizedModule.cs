using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Regions
{
   class RegionizedModule
   {
      private List<SunaRegion> sunaRegions = new List<SunaRegion>();

      public RegionizedModule(SourceModule sourceModule)
      {
         Contract.Requires(sourceModule != null);

         // dummy to suppress a warning for now
         sunaRegions.Add(new SunaRegion());
         throw new NotImplementedException();
      }

      public ICollection<SunaRegion> SunaRegions
      {
         get
         {
            return sunaRegions;
         }
      }
   }
}
