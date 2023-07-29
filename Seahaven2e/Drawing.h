// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_DRAWING_H_
#define SEAHAVEN2E_DRAWING_H_

#include <stdint.h>
#include <Apple2Lib/HGRPage.h>
#include <Apple2Lib/HGRWord.h>
#include "Game.h"
#include "SavedBackground.h"

class Drawing {
public:
   void DrawBackground();
   void DrawCard(CompactCard card, uint8_t x, uint8_t y);
   void DrawCardWithShadow(CompactCard card, uint8_t x, uint8_t y);
   void DrawGame();
   void DrawAcePiles();
   void DrawTowers();

   void EraseCard(CardLocation location);
   void ToggleCursor(uint8_t x, uint8_t y);

   // dispatchers to the HGRPage
   a2::HGRPage GetHGRPage() const { return hgr; }
   void CopyTo(Drawing target) { hgr.CopyTo(target.hgr); }
   void Show() { hgr.Show(); }

public:
   static Drawing Page1();
   static Drawing Page2();

private:
  void DrawAcePile(Suit suit, uint8_t x);
  void DrawCardBottom(uint8_t x, uint8_t y);
  void DrawCardTop(CompactCard card, uint8_t x, uint8_t y);
  void DrawColumns();
  void DrawSprite(
          const a2::HGRWord *sprite,
          uint8_t rows,
          uint8_t y,
          uint8_t x);
  void XorSprite(const a2::HGRWord *sprite, uint8_t rows, uint8_t y, uint8_t x);

private:
  a2::HGRPage hgr;
};

// I pass these around by value because they are just a byte indicating the page
// offset of the HGR memory.  If the size changes we need to be scolded and
// consider the design.
static_assert(sizeof(Drawing) == 1, "class Drawing getting larger, consider redesign");


#endif  // SEAHAVEN2E_DRAWING_H_
