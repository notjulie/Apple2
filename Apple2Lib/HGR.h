
#ifndef APPLE2HGR_H
#define APPLE2HGR_H

#include <stdint.h>

namespace a2 {
   const uint8_t HGRHeight = 192;

   inline uint8_t *HGRPage1Base() {
      return (uint8_t *)0x2000;
   }

   class HGRRow {
   public:
      static void InitializeRowOffsets();

      static inline uint16_t GetIndex(uint8_t row) {
         return rowOffsets[row];
      }

   private:
      static uint16_t rowOffsets[192];
   };

   void HGRFill(uint8_t value);

}

#endif // APPLE2HGR_H
