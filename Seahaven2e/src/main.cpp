
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>

static void SetHGRWhite()
{
   for (int y=0; y<192; ++y)
   {
      uint8_t *p = (uint8_t *)(0x2000 + a2::HGRRow::GetIndex(y));
      for (int x=0; x<40; ++x)
         p[x] = 0xFF;
   }
}

/** \brief
 * Main entry point
 */
extern "C" int main()
{
   // call initilization
   a2::HGRRow::InitializeRowOffsets();

   // dump to the HGR screen
   a2::HIRESON();
   a2::TEXTOFF();
   SetHGRWhite();
}
