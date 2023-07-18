// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

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
  Card card = Game::instance.GetAcePileCard(suit);
  if (!card.IsNull()) {
      DrawCard(card, x, CardLocations::TowersTop);
  }
}

void Drawing::DrawAcePiles() {
  DrawAcePile(Suit::Clubs, 0);
  DrawAcePile(Suit::Diamonds, 4);
  DrawAcePile(Suit::Hearts, 32);
  DrawAcePile(Suit::Spades, 36);
  uint8_t x = TowersLeft;

  for (uint8_t tower=0; tower < 4; ++tower) {
    Card card = Game::instance.GetTower(tower);
    if (!card.IsNull()) {
      DrawCard(card, x, CardLocations::TowersTop);
    }
    x += 4;
  }
}

void Drawing::DrawTowers() {
  uint8_t x = TowersLeft;

  for (uint8_t tower=0; tower < 4; ++tower) {
    Card card = Game::instance.GetTower(tower);
    if (!card.IsNull()) {
      DrawCard(card, x, CardLocations::TowersTop);
    }
    x += 4;
  }
}

/// <summary>
/// Draws all the columns
/// </summary>
void Drawing::DrawColumns() {
  uint8_t x = 0;

  for (uint8_t i=0; i < 10; ++i) {
    Column &column = Game::instance.GetColumn(i);
    uint8_t cardCount = column.GetCount();

    for (uint8_t j=0; j < cardCount; ++j)
      DrawCardTop(column.GetCard(j), x, columnYLookup.Y(j));

    DrawCardBottom(x, columnYLookup.Y(cardCount - 1) + CardTopSpriteHeight);

    x += 4;
  }
}

void Drawing::DrawGame() {
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
  for (int i=0; i<CardHeight; ++i) {
    uint8_t *row = hgr.GetByteAddress(y++, x);
    row[0] = 0;
    row[1] = 0;
    row[2] = 0;
    row[3] = 0;
  }

  // if this was a column card we'll need to redraw the card above it, or at least
  // its lower part
  if (location.IsColumn()) {
    DrawCardBottom(x, startY - CardLocations::CardShadowHeight);
  }
}


/// <summary>
/// Saves a region from an HGR page to the SavedBackground object
/// </summary>
void Drawing::SaveCardBackground(
              uint8_t x, uint8_t y,
              SavedBackground *background) {
  a2::VBLCounter::Update();
  uint8_t *p = &background->pixels[0];
  uint8_t *row;
  for (uint8_t i=0; i < SavedBackground::Height; ++i) {
    row = hgr.GetByteAddress(y++, x);
    p[0] = row[0];
    p[1] = row[1];
    p[2] = row[2];
    p[3] = row[3];
    p += 4;
  }
}


/// <summary>
/// Restores a region of the HGR page from the SavedBackground object
/// </summary>
void Drawing::RestoreBackground(
                SavedBackground *background,
                uint8_t x, uint8_t y) {
  a2::VBLCounter::Update();
  uint8_t *p = &background->pixels[0];
  uint8_t *row;
  for (uint8_t i=0; i < SavedBackground::Height; ++i) {
    row = hgr.GetByteAddress(y++, x);
    row[0] = p[0];
    row[1] = p[1];
    row[2] = p[2];
    row[3] = p[3];
    p += 4;
  }
}
