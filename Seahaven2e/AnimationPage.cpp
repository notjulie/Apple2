// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "AnimationPage.h"


SavedBackground AnimationPage::background1;
SavedBackground AnimationPage::background2;

SavedBackground *AnimationPage::backgrounds[2];
constexpr Drawing AnimationPage::drawings[2];


void AnimationPage::Initialize()
{
   backgrounds[0] = &background1;
   backgrounds[1] = &background2;
}


__attribute__((noinline)) void AnimationPage::MoveCard(CompactCard card, uint8_t x, uint8_t y)
{
   Drawing drawing = GetDrawing();

   // restore the background
   GetBackground().RestoreBackground();

   // save the background at the new location
   GetBackground().SaveCardBackground(drawing.GetHGRPage(), x, y);
   drawing.DrawCardWithShadow(card, x, y);
}


__attribute__((noinline)) void AnimationPage::DrawGame()
{
   Drawing drawing = GetDrawing();

   drawing.DrawBackground();
   drawing.DrawGame();
   GetBackground().Forget();
}


void AnimationPage::CopyFrom(AnimationPage &from)
{
   from.GetDrawing().CopyTo(GetDrawing());
   GetBackground().Forget();
}


void AnimationPage::EraseCard(CardLocation location)
{
   GetDrawing().EraseCard(location);
}


/// <summary>
/// Takes action to clean up after an animation completes
/// </summary>
void AnimationPage::EndAnimation()
{
   // basically we just need to tell ourself not to overwrite
   // the card we just moved next time we're called
   GetBackground().Forget();
}


