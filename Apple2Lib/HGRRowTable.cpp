
#include "HGRRowTable.h"
#include <C6502/Memory.h>

namespace a2 {

   /** \brief
    * Fills the entire HGR window with the given value
    */
   void HGRRowTable::Fill(uint8_t value) const
   {
      for (uint8_t y=0; y<192; ++y)
      {
         uint8_t *p = GetRowAddress(y);
         c6502::memset8(p, value, 40);
      }
   }

}
