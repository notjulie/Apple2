
#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
#include <Apple2Lib/HGRPage.h>
#include <Apple2Lib/HGRWord.h>
#include "Game.h"
#include "SavedBackground.h"

class Drawing {
public:
   constexpr Drawing(a2::HGRPage hgrPage)
      : hgr(hgrPage)
   {
   }

   void DrawBackground();
   void DrawCard(Card card, uint8_t x, uint8_t y);
   void DrawCardBackground(uint8_t row, uint8_t byteOffset);
   void SaveCardBackground(uint8_t x, uint8_t y, SavedBackground *background);
   void DrawGame();
   void DrawAcePiles();
   void DrawTowers();
   void CopyTo(Drawing &target) { hgr.CopyTo(target.hgr); }
   void RestoreBackground(SavedBackground &bacground);

private:
   void DrawAcePile(Suit suit, uint8_t x);
   void DrawCardBottom(uint8_t x, uint8_t y);
   void DrawCardTop(Card card, uint8_t x, uint8_t y);
   void DrawColumns();
   void DrawSprite(const a2::HGRWord *sprite, uint8_t rows, uint8_t y, uint8_t x);

private:
   const a2::HGRPage hgr;
};

extern Drawing drawing1;
extern Drawing drawing2;


#endif // DRAWING_H
