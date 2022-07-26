
#ifndef APPLE2HGR_H
#define APPLE2HGR_H

#include <stdint.h>

namespace a2 {
   const uint8_t HGRHeight = 192;


   class HGRRow {
   public:
      static void InitializeRowOffsets();

      static inline uint16_t GetIndex(uint8_t row) {
         return rowOffsets[row];
      }

   private:
      static uint16_t rowOffsets[192];
   };

}

#endif // APPLE2HGR_H
