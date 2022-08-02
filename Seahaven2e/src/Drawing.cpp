
#include "Drawing.h"
#include <stdint.h>
#include <Apple2Lib/HGRRowTable.h>
#include <C6502/Memory.h>

const uint16_t WWWWWWWWWWWWWW = 0xFFFF;
const uint16_t WRRRRRRRRRRRRR = 0xAAD5;
const uint16_t _RRRRRRRRRRRRR = 0xA8D5;

const a2::HGRRowTable hgr;

/** \brief
 * Draws the shape of a card at the given location in the main
 * HGR window
 */
void DrawCardBackground(uint8_t row, uint8_t byteOffset)
{
   {
      uint8_t *rowPointer = hgr.GetByteAddress(row++, byteOffset);
      rowPointer[0] = 0xFC;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x1F;
   }

   for (uint8_t i=0; i<37; ++i)
   {
      uint8_t *rowPointer = hgr.GetByteAddress(row++, byteOffset);
      rowPointer[0] = 0xFE;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x3F;
   }

   {
      uint8_t *rowPointer = hgr.GetByteAddress(row++, byteOffset);
      rowPointer[0] = 0xFC;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x1F;
   }
}


/** \brief
 * Test that draws a bunch of cards
 */
void DrawABunchOfCards()
{
   //for (uint8_t offset=0; offset<40; offset+=4)
      //DrawCardBackground(10, offset);

   uint16_t pattern = _RRRRRRRRRRRRR;
   uint8_t row = 5;
   uint8_t byteOffset = 10;
   for (uint8_t i=0; i<20; ++i)
   {
      uint8_t *rowPointer = hgr.GetByteAddress(row++, byteOffset);
      rowPointer[0] = c6502::hi8(pattern);
      rowPointer[1] = c6502::lo8(pattern);
      rowPointer[2] = c6502::hi8(pattern);
      rowPointer[3] = c6502::lo8(pattern);
   }
}

void DrawBackground()
{
   hgr.Fill(0);
}


