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

/// <summary>
/// This is a special case move that happens at the end of a
/// column-to-column animation in a case where the card is going
/// to end up behind another card such that only the top will be
/// showing.
/// </summary>
void AnimationPage::MoveCardTop(Card card, CardLocation location)
{
   Drawing drawing = GetDrawing();

   uint8_t x = location.GetX();
   uint8_t y = location.GetY() - CardLocations::CardShadowHeight;

   // restore the background at the old position and save the
   // background at the new position
   SavedBackground::RestoreAndSave(page, x, y);

   // draw the card at the new position
   drawing.DrawCardTopWithShadow(card, x, y, location.GetCardTopHeight() - CardLocations::CardShadowHeight);
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


