
#include "IO.h"

namespace a2 {

   static const uint16_t KBD      = 0xC000;
   //static const uint16_t KBDSTRB  = 0xC010;

   /** \brief
    * Apple2 version of something generally like getchar
    */
   int16_t getchar()
   {
      // get a character if we have one
      int8_t b = *(volatile int8_t *)KBD;
      if (b < 0)
      {
         // got a character; clear the strobe
         asm volatile ("BIT\t$C010" : );
         return 0x7F & b;
      }
      else
      {
         return -1;
      }
   }
}

