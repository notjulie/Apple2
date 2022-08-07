
#ifndef LOOKUPTABLE_H
#define LOOKUPTABLE_H

namespace c6502 {

   /** \brief
    * Simple 16-bit lookup table optimized for 8-bit processors
    */
   template <typename T, int length> class Lookup16Bit {
      static_assert(sizeof(T) == 2, "Lookup16Bit required 16 bit type");

   public:
      constexpr Lookup16Bit() {}

      /** \brief
       * Sets a value in the table
       */
      constexpr void Set(uint8_t index, T value) {
         hi[index] = ((uint16_t)value) >> 8;
         lo[index] = (uint8_t)(uint16_t)value;
      }

      /** \brief
       * Gets a value from the table
       */
      inline T Get(uint8_t index) const {
         return (T)((hi[index]<<8) | lo[index]);
      }

      /** \brief
       * Gets a value from the table
       */
      inline uint8_t GetLowByte(uint8_t index) const {
         return lo[index];
      }

      /** \brief
       * Gets a value from the table
       */
      inline uint8_t GetHighByte(uint8_t index) const {
         return hi[index];
      }

   private:
      uint8_t hi[length];
      uint8_t lo[length];
   };

}


#endif // LOOKUPTABLE_H
