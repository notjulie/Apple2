// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef ANIMATIONPAGE_h
#define ANIMATIONPAGE_h

#include "Drawing.h"


/// <summary>
/// Wrapper around HGRPage with support for animation
/// </summary>
class AnimationPage
{
public:
   AnimationPage() {}

   void CopyFrom(AnimationPage &from);
   void DrawGame();
   void EndAnimation();
   void EraseCard(CardLocation location);
   void MoveCard(CompactCard card, uint8_t x, uint8_t y);

   void Show() { GetDrawing().Show(); }
   Drawing GetDrawing();
   SavedBackground &GetBackground() { return page==0 ? background1 : background2; }

public:
   static AnimationPage Page1() { return AnimationPage(0); }
   static AnimationPage Page2() { return AnimationPage(1); }

private:
   AnimationPage(uint8_t page) : page(page) {}

private:
   // construction parameters
   uint8_t page;

   // our saved backgrounds
   static SavedBackground background1;
   static SavedBackground background2;
};

// this is just a wrapper around an index so that we can try to
// minimize our passing around of pointers... make sure of that
static_assert(sizeof(AnimationPage) == 1, "AnimationPage is too big");

#endif // ANIMATIONPAGE_h
