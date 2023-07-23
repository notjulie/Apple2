
#include "AnimationPage.h"


/// ========================================================================
/// ========================================================================
///     class AnimationPage
/// ========================================================================
/// ========================================================================

void AnimationPage::MoveCard(CompactCard card, uint8_t x, uint8_t y)
{
   // if we have a saved background we need to restore it
   if (backgroundSaved)
   {
      drawing.RestoreBackground(&background, backgroundX, backgroundY);
      backgroundSaved = false;
   }

   // save the background at the new location
   backgroundX = x;
   backgroundY = y;
   drawing.SaveCardBackground(x, y, &background);
   backgroundSaved = true;
   drawing.DrawCardWithShadow(card, x, y);
}


void AnimationPage::DrawGame()
{
   drawing.DrawBackground();
   drawing.DrawGame();
   backgroundSaved = false;
}


void AnimationPage::CopyFrom(AnimationPage &from)
{
   from.drawing.CopyTo(drawing);
   backgroundSaved = false;
}


void AnimationPage::EraseCard(CardLocation location)
{
   drawing.EraseCard(location);
}


/// <summary>
/// Takes action to clean up after an animation completes
/// </summary>
void AnimationPage::EndAnimation()
{
   // basically we just need to tell ourself not to overwrite
   // the card we just moved next time we're called
   backgroundSaved = false;
}
