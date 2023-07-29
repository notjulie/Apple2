
#include "AnimationPage.h"


/// ========================================================================
/// ========================================================================
///     class AnimationPage
/// ========================================================================
/// ========================================================================

void AnimationPage::MoveCard(CompactCard card, uint8_t x, uint8_t y)
{
   // restore the background
   background.RestoreBackground();

   // save the background at the new location
   background.SaveCardBackground(drawing.GetHGRPage(), x, y);
   drawing.DrawCardWithShadow(card, x, y);
}


void AnimationPage::DrawGame()
{
   drawing.DrawBackground();
   drawing.DrawGame();
   background.Forget();
}


void AnimationPage::CopyFrom(AnimationPage &from)
{
   from.drawing.CopyTo(drawing);
   background.Forget();
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
   background.Forget();
}
