
#include "Drawing.h"
#include <stdint.h>
#include <Apple2Lib/HGRRowTable.h>
#include <Apple2Lib/HGRWord.h>
#include <C6502/Memory.h>


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
   for (uint8_t offset=0; offset<40; offset+=4)
      DrawCardBackground(10, offset);
}

void DrawBackground()
{
   hgr.Fill(0);
}

void DrawTest()
{
   static const a2::HGRWord black ("              ");
   static const a2::HGRWord white ("WWWWWWWWWWWWWW");
   static const a2::HGRWord red   ("RRRRRRRRRRRRRR");
   static const a2::HGRWord blue  ("BBBBBBBBBBBBBB");
   static const a2::HGRWord violet("VVVVVVVVVVVVVV");
   static const a2::HGRWord green ("GGGGGGGGGGGGGG");

   uint8_t y = 10;
   uint8_t *row;

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = white.GetLeft();
      row[1] = white.GetRight();
   }

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = black.GetLeft();
      row[1] = black.GetRight();
   }

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = red.GetLeft();
      row[1] = red.GetRight();
   }

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = blue.GetLeft();
      row[1] = blue.GetRight();
   }

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = violet.GetLeft();
      row[1] = violet.GetRight();
   }

   for (uint8_t i=0; i<10; ++i)
   {
      row = hgr.GetByteAddress(y++, 10);
      row[0] = green.GetLeft();
      row[1] = green.GetRight();
   }
}
