
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>


/** \brief
 * Main entry point
 */
extern "C" int main()
{
   a2::HGRRow::InitializeRowOffsets();
   for (int i=0; i<10; ++i)
   {
      uint16_t offset = a2::HGRRow::GetIndex(i);
      a2::PRBYTE(offset>>8);
      a2::PRBYTE((uint8_t)offset);
      a2::CLREOL();
      a2::CR();
   }
}
