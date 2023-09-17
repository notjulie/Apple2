// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "AnimationPage.h"


constexpr Drawing AnimationPage::drawings[2];


__attribute__((noinline)) void AnimationPage::MoveCard(Card card, uint8_t x, uint8_t y)
{
   Drawing drawing = GetDrawing();

   // restore the background at the old position and save the
   // background at the new position
   SavedBackground::RestoreAndSave(page, x, y);

   // draw the card at the new position
   drawing.DrawCardWithShadow(card, x, y);
}

void AnimationPage::MoveCardTop(Card card, uint8_t x, uint8_t y)
{
   Drawing drawing = GetDrawing();

   // restore the background at the old position and save the
   // background at the new position
   SavedBackground::RestoreAndSave(page, x, y);

   // draw the card at the new position
   drawing.DrawCardTopWithShadow(card, x, y);
}

__attribute__((noinline)) void AnimationPage::DrawGame()
{
   Drawing drawing = GetDrawing();

   drawing.DrawBackground();
   drawing.DrawGame();
   ForgetBackground();
}


void AnimationPage::CopyFrom(AnimationPage &from)
{
   from.GetDrawing().CopyTo(GetDrawing());
   ForgetBackground();
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
   ForgetBackground();
}

__attribute__((noinline)) void AnimationPage::ForgetBackground()
{
   SavedBackground::Forget(page);
}


