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

#include "PersistentState.h"
#include "Rank.h"
#include "Suit.h"
#include "Sprites.h"


static const uint8_t TowersLeft = 12;


void Drawing::DrawBackground() {
  hgr.Fill(0);
}


void Drawing::DrawSprite(
                const CardTopSprite &sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x) {
  for (int i=0; i < rows; ++i) {
    uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
    rowPointer[0] = sprite.rows[i].GetLeft();
    rowPointer[1] = sprite.rows[i].GetRight();
  }
}


void Drawing::XorSprite(
                const CardTopSprite &sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x) {
  for (int i=0; i < rows; ++i) {
    uint8_t *rowPointer = hgr.GetByteAddress(y++, x);
    rowPointer[0] ^= sprite.rows[i].GetLeft();
    rowPointer[1] ^= sprite.rows[i].GetRight();
  }
}


void Drawing::DrawCardTop(Card card, uint8_t x, uint8_t y) {
  DrawSprite(Sprites::GetRankSprite(card.GetRank()), CardTopSpriteHeight, y, x);
  DrawSprite(
    Sprites::GetSuitSprite(card.GetSuit(), (bool)(x&1)),
    CardTopSpriteHeight,
    y, x + 2);
}


/// <summary>
/// Toggles the state of the cursor drawn at the current location
/// </summary>
void Drawing::ToggleCursor(CardLocation location)
{
   uint8_t x = location.GetX();
   uint8_t y = location.GetY();

   XorSprite(Sprites::cursorLeft, CardTopSpriteHeight, y, x);
   XorSprite(Sprites::cursorRight, CardTopSpriteHeight, y, x + 2);
}


/// <summary>
/// Draws a card with its shadow
/// </summary>
void Drawing::DrawCardWithShadow(Card card, uint8_t x, uint8_t y) {
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
void Drawing::DrawCardTopWithShadow(Card card, uint8_t x, uint8_t y) {
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

void Drawing::DrawCard(Card card, uint8_t x, uint8_t y) {
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


void Drawing::DrawAcePile(uint8_t suitOrdinal, uint8_t x)
{
   auto &game = PersistentState::instance.Game;

   Rank rank = game.GetAcePileRank(suitOrdinal);
   if (rank != Rank::Null)
   {
      DrawCard(Card(Suit::FromOrdinal(suitOrdinal), rank), x, CardLocations::TowersTop);
   }
}


/// <summary>
/// Draws the ace piles
/// </summary>
__attribute__((noinline)) void Drawing::DrawAcePiles()
{
   DrawAcePile(Suit::Clubs().GetOrdinal(), 0);
   DrawAcePile(Suit::Diamonds().GetOrdinal(), 4);
   DrawAcePile(Suit::Hearts().GetOrdinal(), 32);
   DrawAcePile(Suit::Spades().GetOrdinal(), 36);
}


/// <summary>
/// Draws the towers
/// </summary>
void Drawing::DrawTowers()
{
   auto &game = PersistentState::instance.Game;

   uint8_t x = TowersLeft;

   for (uint8_t tower=0; tower < 4; ++tower)
   {
      Card card = game.GetTower(tower);
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
   auto &game = PersistentState::instance.Game;

   uint8_t x = 0;

   for (uint8_t column=0; column < 10; ++column)
   {
      uint8_t cardCount = game.GetNumberOfCardsOnColumn(column);
      if (cardCount != 0)
      {
         for (uint8_t row=0; row < cardCount; ++row)
            DrawCardTop(game.GetColumnCard(column, row), x, columnYLookup.Y(row));

         DrawCardBottom(x, columnYLookup.Y(cardCount - 1) + CardTopSpriteHeight);
      }

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
      DrawAcePile(location.GetAceSuitOrdinal(), location.GetX());
   }
}

