// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef HGRADDRESSCALCULATOR_H
#define HGRADDRESSCALCULATOR_H

#include <stdint.h>

namespace a2 {

   /// <summary>
   /// Single-instance (publicly static) class for calculating the addresses
   /// of row offsets in a HGR page.  It uses a global constexpr instance
   /// with lookup tables and a little bit of assembly to keep things
   /// lean and quick.
   /// </summary>
   class HGRAddressCalculator {
   private:
      /// <summary>
      /// our constructor for our lookup tables
      /// </summary>
      constexpr HGRAddressCalculator()
         :
            lowLookup(),
            highLookup()
      {
         // since this happens at compile time I'm doing it very plainly
         // for the sake of documentation; this loop goes through and calculates
         // the offsets of the 192 rows sequentially, but we shove them into
         // our outputs as we see fit
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

                  // the low byte changes every 8th row
                  lowLookup[index>>3] = (uint8_t)rowOffset;

                  // the high byte is a 64 byte pattern that repeats
                  // 3 times
                  highLookup[index&0x3F] = rowOffset>>8;
                  index++;
               }
            }
         }
      }

   public:
      /// <summary>
      /// Gets the low byte of the offset to the given row
      /// </summary>
      static uint8_t GetLowByte(uint8_t row) {
         uint8_t result = row;

         // the compiler is not very smart about using the accumulator
         // to do shifting, so I give it a little help here
         // result = row>>3;
         asm volatile (
            "LSR\n"
            "LSR\n"
            "LSR\n"
         : "+a"(result) // outputs
         :  // input
         : // clobbers
         );

         return instance.lowLookup[result];
      }

      /// <summary>
      /// Gets the high byte of the offset to the given row
      /// </summary>
      static uint8_t GetHighByte(uint8_t row) {
         return instance.highLookup[row&0x3F];
      }

   private:
      uint8_t lowLookup[24];
      uint8_t highLookup[64];

   private:
      static const HGRAddressCalculator instance;
   };

}

#endif // HGRADDRESSCALCULATOR_H

