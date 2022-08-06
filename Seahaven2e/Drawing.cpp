
#include "Drawing.h"

#include <stdint.h>
#include <Apple2Lib/HGRRowTable.h>
#include <Apple2Lib/HGRWord.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>

#include "Game.h"
#include "Rank.h"
#include "Suit.h"
#include "Sprites.h"


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
   uint8_t x = 4;
   uint8_t y = 10;

   // suits
   for (auto suit=Suit::Clubs; suit<=Suit::Spades; ++suit)
   {
      DrawSprite(Sprites::GetSuitSprite(suit), CardTopSpriteHeight, y, x);
      y += 10;
   }

   // ranks
   for (auto rank=Rank::Ace; rank<=Rank::King; ++rank)
   {
      DrawSprite(Sprites::GetRankSprite(rank), CardTopSpriteHeight, y, x);
      y += 10;

      if (y > 100)
      {
         y = 10;
         x += 4;
      }
   }
}

void DrawCard(Card &card, uint8_t x, uint8_t y)
{
   DrawSprite(Sprites::GetRankSprite(card.GetRank()), CardTopSpriteHeight, y, x);
   DrawSprite(Sprites::GetSuitSprite(card.GetSuit()), CardTopSpriteHeight, y, x + 2);
}

void DrawGame()
{
   uint8_t x = 0;
   for (uint8_t i=0; i<10; ++i)
   {
      Column &column = Game::instance.GetColumn(i);
      for (uint8_t j=0; j<5; ++j)
         DrawCard(column.GetCard(j), x, j * 10);
      x += 4;
   }
}
