// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef HGRPAGE_H
#define HGRPAGE_H

#include <stdint.h>
#include <C6502/Int6502.h>
#include "HGRAddressCalculator.h"

namespace a2 {

   /// <summary>
   /// One-byte representation of a HGRPage with some handy utilities,
   /// the main one being the ability to calculate quickly the address
   /// of any row.
   /// </summary>
   class HGRPage {
   public:
      HGRPage() {}

      void Fill(uint8_t value) const;
      void CopyTo(HGRPage target) const;
      void Show() const;

   public:
      static constexpr HGRPage HGR() { return HGRPage(Page1MemoryPage); }
      static constexpr HGRPage HGR2() { return HGRPage(Page2MemoryPage); }

   private:
      constexpr HGRPage(uint8_t memoryPage) : pageOffset(memoryPage) {}

   private:
      /// <summary>
      /// Returns the address of the given row
      /// </summary>
      uint8_t *GetRowAddress(uint8_t row) const {
         return GetByteAddress(row, 0);
      }

      /// <summary>
      /// Returns the address of the byte at the given offset
      /// </summary>
      uint8_t *GetByteAddress(uint8_t row, uint8_t byteOffset) const {
         c6502::Int16 result;
         result.lo = HGRAddressCalculator::GetLowByte(row) + byteOffset;
         result.hi = pageOffset | HGRAddressCalculator::GetHighByte(row);
         return (uint8_t *)result.i16;
      }

   private:
      static constexpr uint8_t Page1MemoryPage = 0x20; // address 0x2000
      static constexpr uint8_t Page2MemoryPage = 0x40; // address 0x4000

      uint8_t pageOffset;

      friend class HGRContext;
   };

   // the idea here is to be able to pass it around by reference, so
   // keep it small
   static_assert(sizeof(HGRPage) == 1, "HGRPage is supposed to be lean");
}

#endif // HGRROWTABLE_H
