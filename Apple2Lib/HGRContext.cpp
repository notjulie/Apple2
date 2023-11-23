// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "HGRContext.h"

namespace a2 {

   // =======================================================
   // =======================================================
   //      class HGRContext
   // =======================================================
   // =======================================================

   HGRPage HGRContext::page;
   uint8_t HGRContext::row;
   uint8_t HGRContext::byteOffset;


   __attribute__((noinline)) uint8_t *HGRContext::GetByteAddress()
   {
      return page.GetByteAddress(row, byteOffset);
   }

}
