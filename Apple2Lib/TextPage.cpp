
#include "TextPage.h"

#include <C6502/Int6502.h>
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

   __attribute__((noinline)) uint8_t *TextPage::GetRowAddress(uint8_t row, uint8_t pageOffset)
   {
      c6502::Int16 result;

      asm volatile (
         // remember the row
         "PHA\n"

         // calculate the guts of the low byte
         "AND\t#$18\n"
         "STA\t%0\n"
         "ASL\n"
         "ASL\n"
         "ADC\t%0\n"

         // the row'a low bit becomes the high bit of the
         // low byte
         "ASL\n"
         "STA\t%0\n"

         "PLA\n"
         "LSR\n"
         "ROR\t%0\n"

         // what's left goes to the high byte
         "AND\t#3\n"
         "STA\t%1\n"

         // account for the page offset
         "TXA\n"
         "ORA\t%1\n"
         "STA\t%1\n"

      : "=r"(result.lo), "=r"(result.hi) // outputs
      : "a"(row),"x"(pageOffset) // inputs
      : // clobbers
      );

      return (uint8_t *)result.i16;
   }

   void TextPage::Show()
   {
      if (pageOffset == Page1MemoryPage)
         a2::PAGE2OFF();
      else
         a2::PAGE2ON();
      a2::TEXTON();
   }


   void TextPage::WriteAt(uint8_t x, uint8_t y, const char *s)
   {
      char *dest = (char *)(GetRowAddress(y) + x);
      uint8_t i = 0;
      for (;;)
      {
         char c = s[i];
         if (c == 0)
            break;
         dest[i++] = (c | 0x80);
      }
   }

}

