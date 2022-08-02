
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "Drawing.h"



/** \brief
 * Main entry point
 */
extern "C" int main()
{
   // set HGR
   a2::HIRESON();
   a2::TEXTOFF();

   // dump to the HGR screen
   DrawBackground();
   //DrawABunchOfCards();
   DrawTest();
}

