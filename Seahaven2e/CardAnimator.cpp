
#include "CardAnimator.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
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

   // set the bounds of the animation
   currentX = start.GetX();
   currentY = start.GetY();
   targetX = end.GetX();
   targetY = end.GetY();
   cardToMove = card;

   // draw the game without the card on page 2
   drawing2.DrawBackground();
   drawing2.DrawGame();

   // draw the card at its original position, saving the background
   drawing2.SaveCardBackground(start.GetX(), start.GetY(), &background2);
   drawing2.DrawCard(card, start.GetX(), start.GetY());

   // switch to page 2
   a2::PAGE2ON();

   // set the state
   state = State::Page2Initialized;
}

void CardAnimator::Service()
{
   switch (state)
   {
   case State::Idle:
      break;

   case State::Page2Initialized:
      // copy page 2 to page 1
      drawing2.CopyTo(drawing1);

      // erase the card that we're moving
      drawing1.RestoreBackground(background2);

      // set the new position
      UpdatePosition();

      // save background, draw, switch pages, update state
      drawing1.SaveCardBackground(currentX, currentY, &background1);
      drawing1.DrawCard(cardToMove, currentX, currentY);
      a2::PAGE2OFF();
      state = State::Page1Visible;
      break;

   case State::Page1Visible:
      drawing2.RestoreBackground(background2);
      UpdatePosition();
      drawing2.SaveCardBackground(currentX, currentY, &background2);
      drawing2.DrawCard(cardToMove, currentX, currentY);
      a2::PAGE2ON();
      state = State::Page2Visible;
      break;

   case State::Page2Visible:
      drawing1.RestoreBackground(background1);
      UpdatePosition();
      drawing1.SaveCardBackground(currentX, currentY, &background1);
      drawing1.DrawCard(cardToMove, currentX, currentY);
      a2::PAGE2OFF();
      state = State::Page1Visible;
      break;
   }
}

void CardAnimator::UpdatePosition()
{
   a2::puts("CARDANIMATOR::UPDATEPOSITION");
   a2::PAGE2OFF();
   a2::MONITOR();
}
