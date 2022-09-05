
#include "VBLCounter.h"
#include "IO.h"

namespace a2 {

   int8_t VBLCounter::previousState;
   uint8_t VBLCounter::counter;


   void VBLCounter::Update()
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

