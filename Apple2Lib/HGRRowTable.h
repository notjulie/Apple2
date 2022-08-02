
#ifndef HGRROWTABLE_H
#define HGRROWTABLE_H

#include <stdint.h>

namespace a2 {

   /** \brief
    * Look up table that returns the address of a row in a HGR page given its
    * Y coordinate
    */
   class HGRRowTable {
   public:
      /** \brief
       * Initializes a new instance of class HGRRowTable
       */
      constexpr HGRRowTable()
         :rowOffsetsLow(), rowOffsetsHigh()
      {
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

      /** \brief
       * Returns the address of the given row
       */
      inline uint8_t *GetRowAddress(uint8_t row) const {
         return (uint8_t *)(rowOffsetsLow[row] | (rowOffsetsHigh[row]<<8));
      }

      /** \brief
       * Returns the address of the byte at the given offset
       */
      inline uint8_t *GetByteAddress(uint8_t row, uint8_t byteOffset) const {
         return (uint8_t *)(((uint8_t)(rowOffsetsLow[row] + byteOffset)) | (rowOffsetsHigh[row]<<8));
      }

      void Fill(uint8_t value) const;

   private:
      uint8_t rowOffsetsLow[192];
      uint8_t rowOffsetsHigh[192];
   };

}

#endif // HGRROWTABLE_H
