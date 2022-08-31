
#include "CardAnimator.h"

#include "Drawing.h"
#include "Game.h"

CardAnimator CardAnimator::instance;


void CardAnimator::StartAnimation(Card card, CardLocation start, CardLocation end)
{
   // for now just move instantly
   Game::instance.SetCard(start, Card());
   Game::instance.SetCard(end, card);
   drawing1.DrawBackground();
   drawing1.DrawGame();
}
