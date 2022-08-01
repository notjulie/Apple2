
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

      static inline uint8_t *GetRowAddress(uint8_t row) {
         return (uint8_t *)(rowOffsetsLow[row] | (rowOffsetsHigh[row]<<8));
      }

      static inline uint8_t *GetByteAddress(uint8_t row, uint8_t byteOffset) {
         // we can simplify this... the offset doesn't affect the hight byte so
         // we don't have to do 16-bit math
         return (uint8_t *)(((uint8_t)(rowOffsetsLow[row] + byteOffset)) | (rowOffsetsHigh[row]<<8));
      }

   private:
      static uint8_t rowOffsetsLow[192];
      static uint8_t rowOffsetsHigh[192];
   };

   void HGRFill(uint8_t value);

}

#endif // APPLE2HGR_H
