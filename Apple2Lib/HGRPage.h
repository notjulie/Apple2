
#ifndef HGRPAGE_H
#define HGRPAGE_H

#include <stdint.h>
#include <C6502/LookupTable.h>

namespace a2 {

   /** \brief
    * Look up table that returns the address of a row in a HGR page given its
    * Y coordinate
    */
   class HGRPage {
   public:
      /** \brief
       * Initializes a new instance of class HGRRowTable
       */
      static constexpr c6502::Lookup16Bit<uint16_t, 192> GetLookupTable()
      {
         c6502::Lookup16Bit<uint16_t, 192> rowPointers;

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

                  rowPointers.Set(index++, rowOffset);
               }
            }
         }

         return rowPointers;
      }

      /** \brief
       * Returns the address of the given row
       */
      inline uint8_t *GetRowAddress(uint8_t row) const {
         return GetByteAddress(row, 0);
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
               ((pageOffset + rowPointers.GetHighByte(row)) << 8)
            );
      }

      void Fill(uint8_t value) const;
      void CopyTo(HGRPage target) const;
      void Show() const;

   public:
      static HGRPage HGR();
      static HGRPage HGR2();

   private:
      static constexpr uint8_t Page1MemoryPage = 0x20; // address 0x2000
      static constexpr uint8_t Page2MemoryPage = 0x40; // address 0x4000

      uint8_t pageOffset;

   private:
      static c6502::Lookup16Bit<uint16_t, 192> rowPointers;
   };
}

#endif // HGRROWTABLE_H
