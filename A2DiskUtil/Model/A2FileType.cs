using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.Model
{
   public enum A2FileType
   {
      Text = 0x00,
      IntegerBasic = 0x01,
      ApplesoftBasic = 0x02,
      Binary = 0x04,
      SType = 0x08,
      Relocatable = 0x10,
      AType = 0x20,
      BType = 0x20,
   }
}
