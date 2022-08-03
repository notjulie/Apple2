
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


static const a2::HGRWord club[] {
   "WWWWWWWWWWWWWW",
   "WWWWWW  WWWWWW",
   "WWWWW    WWWWW",
   "WWWWWW  WWWWWW",
   "WWW        WWW",
   "WWW        WWW",
   "WWWWWW  WWWWWW",
   "WWWWW    WWWWW",
   "WWWWWWWWWWWWWW",
};

static const a2::HGRWord spade[] {
   "WWWWWWWWWWWWWW",
   "WWWWWW  WWWWWW",
   "WWWWW    WWWWW",
   "WWW        WWW",
   "WW          WW",
   "WWW        WWW",
   "WWWWWW  WWWWWW",
   "WWWWW    WWWWW",
   "WWWWWWWWWWWWWW",
};

static const a2::HGRWord heart[] {
   "WWWWWWWWWWWWWW",
   "WWWRRRWWRRWWWW",
   "WRRRRRRRRRRRWW",
   "WWRRRRRRRRRWWW",
   "WWWRRRRRRRWWWW",
   "WWWWRRRRRWWWWW",
   "WWWWWRRRWWWWWW",
   "WWWWWWWWWWWWWW",
};

static const a2::HGRWord diamond[] {
   "WWWWWWWWWWWWWW",
   "WWWWWRRRWWWWWW",
   "WWWWRRRRRWWWWW",
   "WWWRRRRRRRWWWW",
   "WWRRRRRRRRRWWW",
   "WWWRRRRRRRWWWW",
   "WWWWRRRRRWWWWW",
   "WWWWWRRRWWWWWW",
   "WWWWWWWWWWWWWW",
};

void DrawSprite(const a2::HGRWord *sprite, uint8_t rows, uint8_t y, uint8_t x)
{
   for (int i=0; i<rows; ++i)
   {
      uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
      rowPointer[0] = sprite[i].GetLeft();
      rowPointer[1] = sprite[i].GetRight();
   }
}

void DrawSprites()
{
   DrawSprite(club, sizeof(club)/sizeof(club[0]), 10, 10);
   DrawSprite(diamond, sizeof(diamond)/sizeof(diamond[0]), 30, 10);
   DrawSprite(heart, sizeof(heart)/sizeof(heart[0]), 50, 10);
   DrawSprite(spade, sizeof(spade)/sizeof(spade[0]), 70, 10);
}
