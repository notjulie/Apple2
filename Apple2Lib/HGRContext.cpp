// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "HGRContext.h"
#include <C6502/Memcpy2D.h>

using c6502::Memcpy2D;

static void SetDestByteAddressAndIncrementRow();
static void SetSourceByteAddressAndIncrementRow();


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

   __attribute__((noinline)) void HGRContext::Set2DCopyDest()
   {
      Memcpy2D::SetDestFunction(SetDestByteAddressAndIncrementRow);
      SetDestByteAddressAndIncrementRow();
   }

   __attribute__((noinline)) void HGRContext::Set2DCopySource()
   {
      Memcpy2D::SetSourceFunction(SetSourceByteAddressAndIncrementRow);
      SetSourceByteAddressAndIncrementRow();
   }
}

static void SetDestByteAddressAndIncrementRow()
{
   Memcpy2D::SetDestPointer(a2::HGRContext::GetByteAddress());
   a2::HGRContext::row++;
}

static void SetSourceByteAddressAndIncrementRow()
{
   Memcpy2D::SetSourcePointer(a2::HGRContext::GetByteAddress());
   a2::HGRContext::row++;
}
