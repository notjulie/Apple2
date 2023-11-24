// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "HGRContext.h"
#include <C6502/Memcpy2D.h>
#include "HGR.h"

using c6502::Memcpy2D;



namespace a2 {

   // =======================================================
   // =======================================================
   //      class HGRContext
   // =======================================================
   // =======================================================

   HGRPage HGRContext::page;
   uint8_t HGRContext::row;
   uint8_t HGRContext::byteOffset;


   /// <summary>
   /// Gets the address of the current location
   /// </summary>
   __attribute__((noinline)) uint8_t *HGRContext::GetByteAddress()
   {
      return page.GetByteAddress(row, byteOffset);
   }


   /// <summary>
   /// Set the destination of Memcpy2D to a rectangle origined at the current position
   /// </summary>
   __attribute__((noinline)) void HGRContext::Set2DCopyDest()
   {
      Memcpy2D::SetDestFunction(SetDestByteAddressAndIncrementRow);
      SetDestByteAddressAndIncrementRow();
   }

   /// <summary>
   /// Set the source of Memcpy2D to a rectangle origined at the current position
   /// </summary>
   __attribute__((noinline)) void HGRContext::Set2DCopySource()
   {
      Memcpy2D::SetSourceFunction(SetSourceByteAddressAndIncrementRow);
      SetSourceByteAddressAndIncrementRow();
   }

   /// <summary>
   /// Row-getter function for Memcpy2D
   /// </summary>
   void HGRContext::SetDestByteAddressAndIncrementRow()
   {
      if (a2::HGRContext::row >= a2::HGRHeight)
      {
         Memcpy2D::Terminate();
         return;
      }

      Memcpy2D::SetDestPointer(a2::HGRContext::GetByteAddress());
      a2::HGRContext::row++;
   }

   /// <summary>
   /// Row-getter function for Memcpy2D
   /// </summary>
   void HGRContext::SetSourceByteAddressAndIncrementRow()
   {
      if (a2::HGRContext::row >= a2::HGRHeight)
      {
         Memcpy2D::Terminate();
         return;
      }

      Memcpy2D::SetSourcePointer(a2::HGRContext::GetByteAddress());
      a2::HGRContext::row++;
   }
}

