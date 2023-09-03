
#ifndef INT6502_H
#define INT6502_H

#include <stdint.h>

namespace c6502 {

   struct Int16 {
      Int16() {}
      template <typename T> Int16(T v) { i16 = (int16_t)v; }

      Int16 &operator++() {
         ++i16;
         return *this;
      }

      union {
         int16_t i16;
         struct {
            uint8_t lo;
            uint8_t hi;
         };
      };
   };

}


#endif // INT6502_H
