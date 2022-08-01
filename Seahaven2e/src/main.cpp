
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
   // call initilization
   a2::HGRRow::InitializeRowOffsets();

   // set HGR
   a2::HIRESON();
   a2::TEXTOFF();

   // dump to the HGR screen
   a2::HGRFill(0x00);

   DrawABunchOfRectangles();
}

