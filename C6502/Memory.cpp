
#include "Memory.h"

namespace c6502 {

   /** \brief
    * If the compiler detects that it can replace a loop with memset, it
    * will do so, and as a result link in the entire runtime.  This is a
    * simple inline replacement for memset when the count < 256.
    */
   void memset8(void *dest, uint8_t value, uint8_t count) {
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
      : "y" // clobbers
      );
   }

   /** \brief
    * If the compiler detects that it can replace a loop with memcpy, it
    * will do so, and as a result link in the entire runtime.  This is a
    * simple inline replacement for memcpy when the count < 256.
    */
   void memcpy8(void *dest, const void *src, uint8_t count) {
      asm volatile (
         // load the count into Y, bail if it's zero
         "LDY\t%2\n"
         "BEQ\t3f\n"
         "BNE\t2f\n"

      "1:\n"
         // copy
         "LDA\t(%1),y\n"
         "STA\t(%0),y\n"
      "2:\n"
         // decrement Y, continue if not zero
         "DEY\n"
         "BNE\t1b\n"

         // one last copy at y = 0
         "LDA\t(%1),y\n"
         "STA\t(%0),y\n"
      "3:\n"

      : // outputs : none
      : "r"(dest), "r"(src),"r"(count) // input
      : "a", "y" // clobbers
      );
   }

};

