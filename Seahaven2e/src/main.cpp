
#include <stdint.h>
#include <Apple2IO.h>
#include <Apple2ROM.h>


/** \brief
 * Main entry point
 */
extern "C" int main()
{
   int i;

   char hello[] = "HELLO WORLD";
   for (i=0; i<sizeof(hello) - 1; ++i)
      a2::COUT(hello[i]);

   a2::TEXTOFF();
   a2::HIRESON();
   a2::HIRESOFF();
   a2::TEXTON();
}
