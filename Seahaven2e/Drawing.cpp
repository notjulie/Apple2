// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Drawing.h"

#include <stdint.h>
#include <Apple2Lib/HGRContext.h>
#include <Apple2Lib/HGRWord.h>
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>
#include <C6502/Memcpy2D.h>

#include "PersistentState.h"
#include "Rank.h"
#include "Suit.h"
#include "Sprites.h"

using c6502::Memcpy2D;


// =====================================================================
// =====================================================================
//     class Drawing
// =====================================================================
// =====================================================================


void Drawing::DrawBackground() {
  hgr.Fill(0);
}


void Drawing::XorSprite(
                const CardTopSprite &sprite,
                uint8_t rows,
                uint8_t y,
                uint8_t x)
{
   // set our context
   a2::HGRContext::page = hgr;
   a2::HGRContext::row = y;
   a2::HGRContext::byteOffset = x;

   // draw
   for (int i=0; i < rows; ++i)
   {
      uint8_t *rowPointer = a2::HGRContext::GetByteAddress();
      rowPointer[0] ^= sprite.rows[i].GetLeft();
      rowPointer[1] ^= sprite.rows[i].GetRight();
      a2::HGRContext::row++;
   }
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
void Drawing::DrawCardWithShadow(Card card, uint8_t x, uint8_t y)
{
   // set our context
   a2::HGRContext::page = hgr;
   DrawingPrimatives::cardX = x;
   DrawingPrimatives::cardY = y + CardLocations::CardShadowHeight;

   // draw
   DrawingPrimatives::DrawCardWithShadow(card);
}


/// <summary>
/// Draws the top of a card with its shadow
/// </summary>
void Drawing::DrawCardTopWithShadow(Card card, uint8_t x, uint8_t y, uint8_t height)
{
   // set our context
   a2::HGRContext::page = hgr;
   DrawingPrimatives::cardX = x;
   DrawingPrimatives::cardY = y + CardLocations::CardShadowHeight;

   // draw
   DrawingPrimatives::DrawCardTopWithShadow(card, height);
}

void Drawing::DrawCard(Card card, uint8_t x, uint8_t y)
{
   a2::HGRContext::page = hgr;
   DrawingPrimatives::cardX = x;
   DrawingPrimatives::cardY = y;
   DrawingPrimatives::DrawCard(card);
}


/// <summary>
/// Draws all cards
/// </summary>
__attribute__((noinline)) void Drawing::DrawGame(Drawing drawing)
{
   // brute force implementation... small but inefficient
   uint8_t i = 0;
   for (;;)
   {
      CardLocation location = CardLocation::FromUint8(i);
      Card card = Game::GetCard(location);
      if (!card.IsNull())
      {
         drawing.DrawCardWithShadow(card, location.GetX(), location.GetY() - CardLocations::CardShadowHeight);
      }

      // increment and check for wraparound... that's when we're done
      ++i;
      if (i == 0)
         break;
   }
}


/// <summary>
/// Erases the location; this also redraws the background if there's a card
/// behind the card being erased
/// </summary>
void Drawing::EraseCard(CardLocation location)
{
   // get the coordinates
   a2::HGRContext::page = hgr;
   DrawingPrimatives::cardX = location.GetX();
   DrawingPrimatives::cardY = location.GetY();

   // turn the card to black
   DrawingPrimatives::EraseCardImage();

   // special cases that have a background
   if (location.IsColumn() && location.GetRow()>0)
   {
      // if this was a column card and there was one above it we'll need to
      // redraw the card above it
      location = CardLocation::Column(location.GetColumn(), location.GetRow() - 1);
   }
   else if (location.IsAce())
   {
      // just redraw the location with the card that's there now
      // location = location;
   }
   else
   {
      // no background to draw
      return;
   }

   // cardX and page are already set
   Card backCard = Game::GetCard(location);
   DrawingPrimatives::cardY = location.GetY() + CardLocations::CardShadowHeight;
   DrawingPrimatives::DrawCard(backCard);
}




// =====================================================================
// =====================================================================
//     class DrawingPrimatives
// =====================================================================
// =====================================================================

uint8_t DrawingPrimatives::cardX;
uint8_t DrawingPrimatives::cardY;


__attribute__((noinline)) void DrawingPrimatives::DrawCard(Card card)
{
   a2::VBLCounter::Update();
   DrawCardTop(card);
   a2::VBLCounter::Update();
   DrawCardBottom();
   a2::VBLCounter::Update();
}


void DrawingPrimatives::DrawCardBottom()
{
   // set our context... the page is already set
   a2::HGRContext::row = cardY + CardTopSpriteHeight;
   a2::HGRContext::byteOffset = cardX;

   // clear Memcpy2D
   Memcpy2D::Init();

   // set our source
   static const uint8_t cardBody[4] = {0xFF, 0xFF, 0xFF, 0xBF};
   Memcpy2D::SetSourcePointer(cardBody);

   // set our destination
   a2::HGRContext::Set2DCopyDest();

   // copy
   Memcpy2D::Copy(CardHeight - CardTopSpriteHeight - 1, 4);

   // do the last row
   static const uint8_t cardTail[4] = {0xFE, 0xFF, 0xFF, 0x9F};
   Memcpy2D::SetSourcePointer(cardTail);
   Memcpy2D::Copy(1, 4);
}

/// <summary>
/// Draws the given card at the current location, obeying the given
/// clipping height
/// </summary>
void DrawingPrimatives::DrawCardTop(Card card, uint8_t height)
{
   DrawSprite(SpriteID::FromRank(card.GetRank()), height, cardY, cardX);
   DrawSprite(
      SpriteID::FromSuit(card.GetSuit(), (bool)(cardX&1)),
      height,
      cardY, cardX + 2);
}


/// <summary>
/// Draws the top of a card with its shadow
/// </summary>
void DrawingPrimatives::DrawCardTopWithShadow(Card card, uint8_t height)
{
   // set our context... the page is already set
   a2::HGRContext::row = cardY;
   a2::HGRContext::byteOffset = cardX;

   // draw the shadow
   for (uint8_t i=0; i < CardLocations::CardShadowHeight; ++i)
   {
      uint8_t *row = a2::HGRContext::GetByteAddress();
      row[0] = 0;
      row[1] = 0;
      row[2] = 0;
      row[3] = 0;

      a2::HGRContext::row++;
   }
   cardY += CardLocations::CardShadowHeight;

   // draw the card top
   DrawCardTop(card, height);
}

void DrawingPrimatives::DrawCardWithShadow(Card card)
{
   // draw
   DrawCardTopWithShadow(card);
   DrawCardBottom();
}

void DrawingPrimatives::DrawSprite(
                SpriteID spriteID,
                uint8_t rows,
                uint8_t y,
                uint8_t x)
{
   // set our context... the page is already set
   a2::HGRContext::row = y;
   a2::HGRContext::byteOffset = x;

   Memcpy2D::Init();
   a2::HGRContext::Set2DCopyDest();

   Memcpy2D::SetSourcePointer(Sprites::GetSprite(spriteID));
   Memcpy2D::SetSourceFunction(c6502::Memcpy2D::IncrementSource);
   Memcpy2D::Copy(rows, 2);
}


/// <summary>
/// Paints the card's area black
/// </summary>
__attribute__((noinline)) void DrawingPrimatives::EraseCardImage()
{
   // set our context... the page is already set
   a2::HGRContext::row = DrawingPrimatives::cardY;
   a2::HGRContext::byteOffset = DrawingPrimatives::cardX;

   // clear Memcpy2D
   Memcpy2D::Init();

   // set our source
   static const uint8_t black[4] = {0,0,0,0};
   Memcpy2D::SetSourcePointer(black);

   // set our destination
   a2::HGRContext::Set2DCopyDest();

   // copy
   Memcpy2D::Copy(CardHeight, 4);
}

