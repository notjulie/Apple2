
#include <stdint.h>

// there's no point in returning a result from main in an Apple][ program,
// so that's just 3 wasted op codes
#pragma clang diagnostic ignored "-Wmain-return-type"

inline void putc(register uint8_t c)
{
   asm volatile ("JSR\t$FDED" : "+a"(c));
}

/** \brief
 * Main entry point
 */
void main (void)
{
   int i;

   char hello[] = "HELLO WORLD";
   for (i=0; i<sizeof(hello) - 1; ++i)
      putc(hello[i]);
}
