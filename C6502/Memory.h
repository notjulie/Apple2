
#ifndef C6502_MEMORY_H
#define C6502_MEMORY_H

#include <stdint.h>

namespace c6502 {
   inline uint8_t lo8(uint16_t word) {
      return (uint8_t)word;
   }

   inline uint8_t hi8(uint16_t word) {
      return (uint8_t)(word>>8);
   }

   /** \brief
    * If the compiler detects that it can replace a loop with memset, it
    * will do so, and as a result link in the entire runtime.  This is a
    * simple inline replacement for memset when the count < 256.
    */
   void memset8(void *dest, uint8_t value, uint8_t count);

   /** \brief
    * If the compiler detects that it can replace a loop with memcpy, it
    * will do so, and as a result link in the entire runtime.  This is a
    * simple inline replacement for memcpy when the count < 256.
    */
   void memcpy8(void *dest, const void *src, uint8_t count);
}

#endif // C6502_MEMORY_H

