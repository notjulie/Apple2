
#include "HGRPage.h"
#include <C6502/Memory.h>

namespace a2 {

   c6502::Lookup16Bit<uint16_t, 192> HGRPage::rowPointers = HGRPage::GetLookupTable();

   /** \brief
    * Fills the entire HGR window with the given value
    */
   void HGRPage::Fill(uint8_t value) const
   {
      for (uint8_t y=0; y<192; ++y)
      {
         uint8_t *p = GetRowAddress(y);
         c6502::memset8(p, value, 40);
      }
   }

}
