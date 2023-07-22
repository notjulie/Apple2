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
   explicit constexpr Drawing(a2::HGRPage hgrPage)
    : hgr(hgrPage) {
   }

   void DrawBackground();
   void DrawCard(CompactCard card, uint8_t x, uint8_t y);
   void DrawCardWithShadow(CompactCard card, uint8_t x, uint8_t y);
   void DrawGame();
   void DrawAcePiles();
   void DrawTowers();

   void EraseCard(CardLocation location);

   void CopyTo(Drawing *target) { hgr.CopyTo(target->hgr); }

   void SaveCardBackground(uint8_t x, uint8_t y, SavedBackground *background);
   void RestoreBackground(SavedBackground *background, uint8_t x, uint8_t y);

   void ToggleCursor(uint8_t x, uint8_t y);

   void Show() { hgr.Show(); }

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
  const a2::HGRPage hgr;
};

extern Drawing drawing1;
extern Drawing drawing2;


#endif  // SEAHAVEN2E_DRAWING_H_
