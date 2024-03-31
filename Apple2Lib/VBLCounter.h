
#ifndef VBLCOUNTER_H
#define VBLCOUNTER_H

#include <stdint.h>
#include <C6502/Int6502.h>

namespace a2 {
   class VBLCounter {
   public:
      static const c6502::Int16 &GetCounter() { return counter; }
      static void Update();

   private:
      static int8_t previousState;
      static c6502::Int16 counter;
   };
}

#endif // VBLCOUNTER_H
