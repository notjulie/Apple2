// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Drawing.h"

#include <stdint.h>
#include <Apple2Lib/HGRPage.h>
#include <Apple2Lib/HGRWord.h>
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>

#include "Game.h"
#include "Rank.h"
#include "Suit.h"
#include "Sprites.h"


static const uint8_t TowersLeft = 12;


void Drawing::DrawBackground() {
  hgr.Fill(0);
}


void Drawing::DrawSprite(
                const a2::HGRWord *sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x) {
  for (int i=0; i < rows; ++i) {
    uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
    rowPointer[0] = sprite[i].GetLeft();
    rowPointer[1] = sprite[i].GetRight();
  }
}


void Drawing::XorSprite(
                const a2::HGRWord *sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x) {
  for (int i=0; i < rows; ++i) {
    uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
    rowPointer[0] ^= sprite[i].GetLeft();
    rowPointer[1] ^= sprite[i].GetRight();
  }
}


void Drawing::DrawCardTop(CompactCard card, uint8_t x, uint8_t y) {
  DrawSprite(Sprites::GetRankSprite(card.GetRank()), CardTopSpriteHeight, y, x);
  DrawSprite(
    Sprites::GetSuitSprite(card.GetSuit()),
    CardTopSpriteHeight,
    y, x + 2);
}


void Drawing::ToggleCursor(uint8_t x, uint8_t y) {
  XorSprite(Sprites::cursorLeft, CardTopSpriteHeight, y, x);
  XorSprite(Sprites::cursorRight, CardTopSpriteHeight, y, x + 2);
}


/// <summary>
/// Draws a card with its shadow
/// </summary>
void Drawing::DrawCardWithShadow(CompactCard card, uint8_t x, uint8_t y) {
  // draw the shadow
  for (uint8_t i=0; i < CardLocations::CardShadowHeight; ++i) {
    uint8_t *row = hgr.GetByteAddress(y++, x);
    row[0] = 0;
    row[1] = 0;
    row[2] = 0;
    row[3] = 0;
  }

  // draw the card
  DrawCard(card, x, y);
}


/// <summary>
/// Draws the top of a card with its shadow
/// </summary>
void Drawing::DrawCardTopWithShadow(CompactCard card, uint8_t x, uint8_t y) {
  // draw the shadow
  for (uint8_t i=0; i < CardLocations::CardShadowHeight; ++i) {
    uint8_t *row = hgr.GetByteAddress(y++, x);
    row[0] = 0;
    row[1] = 0;
    row[2] = 0;
    row[3] = 0;
  }

  // draw the card top
  DrawCardTop(card, x, y);
}

void Drawing::DrawCard(CompactCard card, uint8_t x, uint8_t y) {
  a2::VBLCounter::Update();
  DrawCardTop(card, x, y);
  a2::VBLCounter::Update();
  DrawCardBottom(x, y + CardTopSpriteHeight);
  a2::VBLCounter::Update();
}

void Drawing::DrawCardBottom(uint8_t x, uint8_t y) {
  uint8_t * row;

  for (uint8_t i=0; i < CardHeight - CardTopSpriteHeight - 1; ++i) {
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


void Drawing::DrawAcePile(Suit suit, uint8_t x) {
   Rank rank = Game::instance.GetAcePileRank(suit);
   if (rank != Rank::Null)
   {
      DrawCard(CompactCard(suit, rank), x, CardLocations::TowersTop);
   }
}


/// <summary>
/// Draws the ace piles
/// </summary>
__attribute__((noinline)) void Drawing::DrawAcePiles()
{
   DrawAcePile(Suit::Clubs, 0);
   DrawAcePile(Suit::Diamonds, 4);
   DrawAcePile(Suit::Hearts, 32);
   DrawAcePile(Suit::Spades, 36);
}


/// <summary>
/// Draws the towers
/// </summary>
void Drawing::DrawTowers()
{
   uint8_t x = TowersLeft;

   for (uint8_t tower=0; tower < 4; ++tower)
   {
      CompactCard card = Game::instance.GetTower(tower);
      if (!card.IsNull())
      {
         DrawCard(card, x, CardLocations::TowersTop);
      }
      x += 4;
   }
}

/// <summary>
/// Draws all the columns
/// </summary>
void Drawing::DrawColumns()
{
   uint8_t x = 0;

   for (uint8_t column=0; column < 10; ++column)
   {
      uint8_t cardCount = Game::instance.GetNumberOfCardsOnColumn(column);
      for (uint8_t row=0; row < cardCount; ++row)
         DrawCardTop(Game::instance.GetColumnCard(column, row), x, columnYLookup.Y(row));

      DrawCardBottom(x, columnYLookup.Y(cardCount - 1) + CardTopSpriteHeight);

      x += 4;
  }
}


/// <summary>
/// Draws all cards
/// </summary>
__attribute__((noinline)) void Drawing::DrawGame()
{
  DrawAcePiles();
  DrawColumns();
  DrawTowers();
}


/// <summary>
/// Erases the location
/// </summary>
void Drawing::EraseCard(CardLocation location) {
   // get the coordinates
   uint8_t x = location.GetX();
   uint8_t startY = location.GetY();
   uint8_t y = startY;

   // erase the card
   for (int i=0; i<CardHeight; ++i)
   {
      uint8_t *row = hgr.GetByteAddress(y++, x);
      row[0] = 0;
      row[1] = 0;
      row[2] = 0;
      row[3] = 0;
   }

   // special cases
   if (location.IsColumn() && location.GetRow()>0)
   {
      // if this was a column card and there was one above it we'll need to
      // redraw the card above it, or at least its lower part
      DrawCardBottom(x, startY - CardLocations::CardShadowHeight);
   }
   else if (location.IsAce())
   {
      // erasing a card from the ace pile just exposes the card
      // below
      DrawAcePile(location.GetAceSuit(), location.GetX());
   }
}

