
#include "HGR.h"

namespace a2 {

   uint16_t HGRRow::rowOffsets[192];

   void HGRRow::InitializeRowOffsets() {
      uint8_t index = 0;
      for (uint8_t i=0; i<=120; i+=40)
      {
         for (uint8_t j=0; j<8; ++j)
         {
            for (uint8_t k=0; k<8; ++k)
               rowOffsets[index++] =
                  i |
                  (j << 7) |
                  (k << 10)
               ;
         }
      }
   }
}


