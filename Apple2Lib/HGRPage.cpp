// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "HGRPage.h"
#include <C6502/Memory.h>
#include "MMIO.h"
#include "ROM.h"

namespace a2 {

   /// \brief
   ///   Fills the entire HGR window with the given value
   ///
   void HGRPage::Fill(uint8_t value) const
   {
      for (uint8_t y=0; y<192; ++y)
      {
         uint8_t *p = GetRowAddress(y);
         c6502::memset8(p, value, 40);
      }
   }

   /// \brief
   ///   Copies the contents of our page to another page
   ///
   void HGRPage::CopyTo(HGRPage target) const
   {
      uint8_t *dest = (uint8_t *)(target.pageOffset << 8);
      uint8_t *src = (uint8_t *)(pageOffset << 8);
      for (uint8_t i=0; i<64; ++i)
      {
         c6502::memcpy8(dest, src, 128);
         dest += 128;
         src += 128;
      }
   }

   void HGRPage::Show() const
   {
      if (pageOffset == Page2MemoryPage)
      {
         a2::PAGE2ON();
      }
      else
      {
         a2::PAGE2OFF();
      }
   }

}
