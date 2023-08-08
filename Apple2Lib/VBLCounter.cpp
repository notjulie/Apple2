
#include "VBLCounter.h"
#include "MMIO.h"

namespace a2 {

   int8_t VBLCounter::previousState;
   uint8_t VBLCounter::counter;


   __attribute__((noinline)) void VBLCounter::Update()
   {
      int8_t vblState = a2::VBLFLAG();
      if (vblState>=0)
      {
         if (previousState < 0)
         {
            ++counter;
            previousState = vblState;
         }
      }
      else
      {
         previousState = vblState;
      }
   }
}

