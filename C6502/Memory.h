
#ifndef C6502_MEMORY_H
#define C6502_MEMORY_H

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
   inline void memset8(void *dest, uint8_t value, uint8_t count) {
      asm volatile (
         // load the count into Y, bail if it's zero
         "LDY\t%2\n"
         "BEQ\t2f\n"

      "1:\n"
         // decrement our counter, store accumulator continue if y != 0
         "DEY\n"
         "STA\t(%0),y\n"
         "BNE\t1b\n"
      "2:\n"

      : // outputs : none
      : "r"(dest), "a"(value),"r"(count) // input
      : "a", "y" // clobbers
      );
   }
}

#endif // C6502_MEMORY_H

