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
   void DrawCard(Card card, uint8_t x, uint8_t y);
   void DrawCardWithShadow(Card card, uint8_t x, uint8_t y);
   void DrawCardTopWithShadow(Card card, uint8_t x, uint8_t y);
   void DrawGame();
   void DrawAcePiles();
   void DrawTowers();

   void EraseCard(CardLocation location);
   void ToggleCursor(CardLocation location);

   // dispatchers to the HGRPage
   a2::HGRPage GetHGRPage() const { return hgr; }
   void CopyTo(Drawing target) { hgr.CopyTo(target.hgr); }
   void Show() { hgr.Show(); }

public:
   static constexpr Drawing Page1() { return Drawing(a2::HGRPage::HGR()); }
   static constexpr Drawing Page2() { return Drawing(a2::HGRPage::HGR2()); }

private:
   constexpr Drawing(a2::HGRPage hgr) : hgr(hgr) {}
   void DrawAcePile(uint8_t suitOrdinal, uint8_t x);
   void DrawCardBottom(uint8_t x, uint8_t y);
   void DrawCardTop(Card card, uint8_t x, uint8_t y);
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
