
#include "CardAnimator.h"

#include <Apple2Lib/IO.h>
#include "Drawing.h"
#include "Game.h"

CardAnimator CardAnimator::instance;

/// \brief
///   Starts an animation of a card from one position to another
///
void CardAnimator::StartAnimation(Card card, CardLocation start, CardLocation end)
{
   // step 1: remove the card from its current position
   Game::instance.SetCard(start, Card());

   // draw the game without the card on page 2
   drawing2.DrawBackground();
   drawing2.DrawGame();

   // draw the card at its original position, saving the background
   drawing2.DrawCardSavingBackground(card, start.GetX(), start.GetY(), &background2);

   // switch to page 2
   a2::PAGE2ON();
}
