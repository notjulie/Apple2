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

/// <summary>
/// Static class for stateful drawing so that I can reduce parameter
/// passing by setting a current destination for multiple calls
/// </summary>
class DrawingPrimatives {
public:
   // pseudo-parameters... they are global variables that are easier to
   // pass around than parameters
   static a2::HGRPage page;
   static uint8_t cardX;
   static uint8_t cardY;

public:
   static void DrawCard(Card card);
   static void DrawCardTop(Card card);
   static void DrawCardBottom();
   static void DrawColumns();
   static void DrawTowers();
   static void EraseCardImage();

private:
   static void DrawSprite(
          const CardTopSprite &sprite,
          uint8_t rows,
          uint8_t y,
          uint8_t x);
};


/// <summary>
/// More conventional C++ drawing class
/// </summary>
class Drawing {
public:
   void DrawBackground();
   void DrawCard(Card card, uint8_t x, uint8_t y);
   void DrawCardWithShadow(Card card, uint8_t x, uint8_t y);
   void DrawCardTopWithShadow(Card card, uint8_t x, uint8_t y);
   void DrawGame();
   void DrawAcePiles();

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
   void DrawAcePile(uint8_t suitOrdinal);
   void DrawCardTop(Card card, uint8_t x, uint8_t y);
   void XorSprite(const CardTopSprite &sprite, uint8_t rows, uint8_t y, uint8_t x);

private:
   a2::HGRPage hgr;
};

// I pass these around by value because they are just a byte indicating the page
// offset of the HGR memory.  If the size changes we need to be scolded and
// consider the design.
static_assert(sizeof(Drawing) == 1, "class Drawing getting larger, consider redesign");


#endif  // SEAHAVEN2E_DRAWING_H_
