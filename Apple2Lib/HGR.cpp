
#include "HGR.h"
#include <C6502/Memory.h>

namespace a2 {

   uint8_t HGRRow::rowOffsetsLow[192];
   uint8_t HGRRow::rowOffsetsHigh[192];

   void HGRRow::InitializeRowOffsets() {
      uint8_t index = 0;
      for (uint8_t i=0; i<120; i+=40)
      {
         for (uint8_t j=0; j<8; ++j)
         {
            for (uint8_t k=0; k<8; ++k)
            {
               uint16_t rowOffset =
                  i |
                  (j << 7) |
                  (k << 10)
               ;
               rowOffsetsLow[index] = (uint8_t)rowOffset;
               rowOffsetsHigh[index] = 0x20 + (uint8_t)(rowOffset>>8);
               ++index;
            }
         }
      }
   }

   void HGRFill(uint8_t value)
   {
      for (int y=0; y<192; ++y)
      {
         uint8_t *p = a2::HGRRow::GetRowAddress(y);
         c6502::memset8(p, value, 40);
      }
   }
}


