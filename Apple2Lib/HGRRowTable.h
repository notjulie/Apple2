
#ifndef HGRROWTABLE_H
#define HGRROWTABLE_H

#include <stdint.h>
#include <C6502/LookupTable.h>

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

                  rowPointers.Set(index++, (uint8_t *)(0x2000 + rowOffset));
               }
            }
         }
      }

      /** \brief
       * Returns the address of the given row
       */
      inline uint8_t *GetRowAddress(uint8_t row) const {
         return rowPointers.Get(row);
      }

      /** \brief
       * Returns the address of the byte at the given offset
       */
      inline uint8_t *GetByteAddress(uint8_t row, uint8_t byteOffset) const {
         // NOTE: this has a cheat... it adds the offset to the low byte
         // but does not add the carry to the high byte, which is an important
         // performance improvement
         return (uint8_t *)
            (
               (uint8_t)(rowPointers.GetLowByte(row) + byteOffset) |
               (rowPointers.GetHighByte(row) << 8)
            );
      }

      void Fill(uint8_t value) const;

   private:
      c6502::Lookup16Bit<uint8_t *, 192> rowPointers;
   };

}

#endif // HGRROWTABLE_H
