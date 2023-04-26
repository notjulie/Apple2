
#include "Drawing.h"

#include <stdint.h>
#include <Apple2Lib/HGRPage.h>
#include <Apple2Lib/HGRWord.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>

#include "Game.h"
#include "Rank.h"
#include "Suit.h"
#include "Sprites.h"


static const uint8_t TowersLeft = 12;


Drawing drawing1(a2::HGRPage::HGR());
Drawing drawing2(a2::HGRPage::HGR2());

/** \brief
 * Draws the shape of a card at the given location in the main
 * HGR window
 */
void Drawing::DrawCardBackground(uint8_t row, uint8_t byteOffset)
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


void Drawing::DrawBackground()
{
   hgr.Fill(0);
}


void Drawing::DrawSprite(const a2::HGRWord *sprite, uint8_t rows, uint8_t y, uint8_t x)
{
   for (int i=0; i<rows; ++i)
   {
      uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
      rowPointer[0] = sprite[i].GetLeft();
      rowPointer[1] = sprite[i].GetRight();
   }
}


void Drawing::XorSprite(const a2::HGRWord *sprite, uint8_t rows, uint8_t y, uint8_t x)
{
   for (int i=0; i<rows; ++i)
   {
      uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
      rowPointer[0] ^= sprite[i].GetLeft();
      rowPointer[1] ^= sprite[i].GetRight();
   }
}


void Drawing::DrawCardTop(Card card, uint8_t x, uint8_t y)
{
   DrawSprite(Sprites::GetRankSprite(card.GetRank()), CardTopSpriteHeight, y, x);
   DrawSprite(Sprites::GetSuitSprite(card.GetSuit()), CardTopSpriteHeight, y, x + 2);

   DrawCursor(x, y);
}


void Drawing::DrawCursor(uint8_t x, uint8_t y)
{
   XorSprite(Sprites::cursorLeft, CardTopSpriteHeight, y, x);
   XorSprite(Sprites::cursorRight, CardTopSpriteHeight, y, x + 2);
}


void Drawing::DrawCard(Card card, uint8_t x, uint8_t y)
{
   a2::VBLCounter::Update();
   DrawCardTop(card, x, y);
   a2::VBLCounter::Update();
   DrawCardBottom(x, y + CardTopSpriteHeight);
   a2::VBLCounter::Update();
}

void Drawing::DrawCardBottom(uint8_t x, uint8_t y)
{
   uint8_t * row;

   for (uint8_t i=0; i<CardHeight - CardTopSpriteHeight - 1; ++i)
   {
      row = hgr.GetByteAddress(y++, x);
      row[0] = 0xFF;
      row[1] = 0xFF;
      row[2] = 0xFF;
      row[3] = 0xBF;
   }

   row = hgr.GetByteAddress(y++, x);
   row[0] = 0xFE;
   row[1] = 0xFF;
   row[2] = 0xFF;
   row[3] = 0x9F;
}


void Drawing::DrawAcePile(Suit suit, uint8_t x)
{
   Card card = Game::instance.GetAcePileCard(suit);
   if (!card.IsNull())
   {
      DrawCard(card, x, CardLocation::TowersTop);
   }
}

void Drawing::DrawAcePiles()
{
   DrawAcePile(Suit::Clubs, 0);
   DrawAcePile(Suit::Diamonds, 4);
   DrawAcePile(Suit::Hearts, 32);
   DrawAcePile(Suit::Spades, 36);
   uint8_t x = TowersLeft;

   for (uint8_t tower=0; tower<4; ++tower)
   {
      Card card = Game::instance.GetTower(tower);
      if (!card.IsNull())
      {
         DrawCard(card, x, CardLocation::TowersTop);
      }
      x += 4;
   }
}

void Drawing::DrawTowers()
{
   uint8_t x = TowersLeft;

   for (uint8_t tower=0; tower<4; ++tower)
   {
      Card card = Game::instance.GetTower(tower);
      if (!card.IsNull())
      {
         DrawCard(card, x, CardLocation::TowersTop);
      }
      x += 4;
   }
}

void Drawing::DrawColumns()
{
   uint8_t x = 0;

   for (uint8_t i=0; i<10; ++i)
   {
      Column &column = Game::instance.GetColumn(i);

      uint8_t y = CardLocation::ColumnsTop;
      for (uint8_t j=0; j<5; ++j)
      {
         DrawCardTop(column.GetCard(j), x, y);
         if (j == 4)
         {
            DrawCardBottom(x, y + CardTopSpriteHeight);
         }
         else
         {
            y += CardTopSpriteHeight + 1;
         }
      }

      x += 4;
   }
}

void Drawing::DrawGame()
{
   DrawAcePiles();
   DrawColumns();
   DrawTowers();
}


void Drawing::SaveCardBackground(uint8_t x, uint8_t y, SavedBackground *background)
{
   a2::VBLCounter::Update();
   uint8_t *p = &background->pixels[0];
   uint8_t *row;
   for (uint8_t i=0; i<CardHeight; ++i)
   {
      row = hgr.GetByteAddress(y++, x);
      p[0] = row[0];
      p[1] = row[1];
      p[2] = row[2];
      p[3] = row[3];
      p += 4;
   }
}


void Drawing::RestoreBackground(SavedBackground &background, uint8_t x, uint8_t y)
{
   a2::VBLCounter::Update();
   uint8_t *p = &background.pixels[0];
   uint8_t *row;
   for (uint8_t i=0; i<CardHeight; ++i)
   {
      row = hgr.GetByteAddress(y++, x);
      row[0] = p[0];
      row[1] = p[1];
      row[2] = p[2];
      row[3] = p[3];
      p += 4;
   }
}

