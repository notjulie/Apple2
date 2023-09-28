
#ifndef MEMCPY2D_H
#define MEMCPY2D_H

#include <stdint.h>
#include "Int6502.h"

namespace c6502 {

   class Memcpy2D {
   public:
      typedef void Function();

      static void Copy(uint8_t rows, uint8_t rowLength);

      static void SetDestFunction(Function *function);
      static void SetDestPointer(c6502::Int16 dest);
      static void SetSourceFunction(Function *function);
      static void SetSourcePointer(c6502::Int16 source);

      static void IncrementDest();
   };

}

#endif // MEMCPY2D_H
