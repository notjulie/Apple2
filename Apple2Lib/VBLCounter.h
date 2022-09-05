
#ifndef VBLCOUNTER_H
#define VBLCOUNTER_H

#include <stdint.h>

namespace a2 {
   class VBLCounter {
   public:
      static uint8_t GetCounter() { return counter; }
      static void Update();

   private:
      static int8_t previousState;
      static uint8_t counter;
   };
}

#endif // VBLCOUNTER_H
