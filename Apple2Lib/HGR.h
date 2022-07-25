
#ifndef APPLE2HGR_H
#define APPLE2HGR_H

#include <stdint.h>

namespace a2 {
   const uint8_t HGRHeight = 192;


   class HGRRow {
   public:
      static void InitializeRowOffsets() {
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

      static inline uint16_t GetIndex(uint8_t row) {
         return rowOffsets[row];
      }

   private:
      static uint16_t rowOffsets[192];
   };

}

#endif // APPLE2HGR_H
