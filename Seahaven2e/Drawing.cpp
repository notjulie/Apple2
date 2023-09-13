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

a2::HGRPage Drawing::page;
uint8_t Drawing::cardX;
uint8_t Drawing::cardY;


void Drawing::DrawBackground() {
  hgr.Fill(0);
}


void Drawing::StaticDrawSprite(
                const CardTopSprite &sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x) {
   for (int i=0; i < rows; ++i)
   {
    uint8_t *rowPointer = page.GetByteAddress(y++, x);
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


void Drawing::DrawCardTop(Card card, uint8_t x, uint8_t y)
{
   page = hgr;
   cardX = x;
   cardY = y;
   StaticDrawCardTop(card);
}


void Drawing::StaticDrawCardTop(Card card)
{
   StaticDrawSprite(Sprites::GetRankSprite(card.GetRank()), CardTopSpriteHeight, cardY, cardX);
   StaticDrawSprite(
      Sprites::GetSuitSprite(card.GetSuit(), (bool)(cardX&1)),
      CardTopSpriteHeight,
      cardY, cardX + 2);
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

void Drawing::DrawCard(Card card, uint8_t x, uint8_t y)
{
   page = hgr;
   cardX = x;
   cardY = y;
   StaticDrawCard(card);
}

void Drawing::StaticDrawCard(Card card)
{
   a2::VBLCounter::Update();
   StaticDrawCardTop(card);
   a2::VBLCounter::Update();
   StaticDrawCardBottom();
   a2::VBLCounter::Update();
}

void Drawing::DrawCardBottom(uint8_t x, uint8_t y)
{
   page = hgr;
   cardX = x;
   cardY = y - CardTopSpriteHeight;
   StaticDrawCardBottom();
}


void Drawing::StaticDrawCardBottom()
{
  uint8_t * row;

  uint8_t y = cardY + CardTopSpriteHeight;

  for (uint8_t i=0; i < CardHeight - CardTopSpriteHeight - 1; ++i) {
    row = page.GetByteAddress(y++, cardX);
    row[0] = 0xFF;
    row[1] = 0xFF;
    row[2] = 0xFF;
    row[3] = 0xBF;
  }

  row = page.GetByteAddress(y++, cardX);
  row[0] = 0xFE;
  row[1] = 0xFF;
  row[2] = 0xFF;
  row[3] = 0x9F;
}


__attribute__((noinline)) void Drawing::DrawAcePile(uint8_t suitOrdinal, uint8_t x)
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
void Drawing::StaticDrawTowers()
{
   auto &game = PersistentState::instance.Game;

   cardX = TowersLeft;
   cardY = CardLocations::TowersTop;

   for (uint8_t tower=0; tower < 4; ++tower)
   {
      Card card = game.GetTower(tower);
      if (!card.IsNull())
      {
         StaticDrawCard(card);
      }
      cardX += 4;
   }
}

/// <summary>
/// Draws all the columns
/// </summary>
void Drawing::StaticDrawColumns()
{
   auto &game = PersistentState::instance.Game;

   cardX = 0;

   for (uint8_t column=0; column < 10; ++column)
   {
      uint8_t cardCount = game.GetNumberOfCardsOnColumn(column);
      if (cardCount != 0)
      {
         for (uint8_t row=0; row < cardCount; ++row)
         {
            cardY = columnYLookup.Y(row);
            StaticDrawCardTop(game.GetColumnCard(column, row));
         }

         StaticDrawCardBottom();
      }

      cardX += 4;
  }
}


/// <summary>
/// Draws all cards
/// </summary>
__attribute__((noinline)) void Drawing::DrawGame()
{
   page = hgr;
   DrawAcePiles();
   StaticDrawColumns();
   StaticDrawTowers();
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

