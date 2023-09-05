
#include "TextPage.h"

#include <C6502/Memory.h>
#include "MMIO.h"


namespace a2 {
   void TextPage::Clear()
   {
      for (uint8_t row=0; row<24; ++row)
      {
         c6502::memset8(GetRowAddress(row), 0xA0, 40);
      }
   }

   uint8_t *TextPage::GetRowAddress(uint8_t row)
   {
      uint8_t group = row & ~7;
      return reinterpret_cast<uint8_t *>(
         (pageOffset << 8) |
         (group + (group<<2)) |
         ((row & 7) << 7)
      );
   }

   void TextPage::Show()
   {
      if (pageOffset == Page1MemoryPage)
         a2::PAGE2OFF();
      else
         a2::PAGE2ON();
      a2::TEXTON();
   }
}

