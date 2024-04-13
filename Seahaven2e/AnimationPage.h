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
   void MoveCard(Card card, uint8_t x, uint8_t y);
   void MoveCardTop(Card card, CardLocation location);
   void ForgetBackground();

   void Show() { GetDrawing().Show(); }
   __attribute__((noinline)) Drawing GetDrawing() { return drawings[page]; }

public:
   static AnimationPage Page1() { return AnimationPage(0); }
   static AnimationPage Page2() { return AnimationPage(1); }

private:
   AnimationPage(uint8_t page) : page(page) {}

private:
   // construction parameters
   uint8_t page;

   // our lookups
   static constexpr Drawing drawings[2] = { Drawing::Page1(), Drawing::Page2() };
};

// this is just a wrapper around an index so that we can try to
// minimize our passing around of pointers... make sure of that
static_assert(sizeof(AnimationPage) == 1, "AnimationPage is too big");

#endif // ANIMATIONPAGE_h
