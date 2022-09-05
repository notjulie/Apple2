
#include "StateMachine.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "CardAnimator.h"
#include "CardLocation.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"

void StateMachine::Service()
{
   switch (state)
   {
   case State::Uninitialized:
      // set HGR
      a2::HIRESON();
      a2::TEXTOFF();
      a2::MIXEDON();

      // new game
      Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());

      // dump to the HGR screen
      drawing1.DrawBackground();
      drawing1.DrawGame();

      // switch to idle state for now
      state = State::Idle;
      break;

   case State::Idle:
      ServiceIdle();
      break;

   case State::Animating:
      CardAnimator::instance.Service();
      if (!CardAnimator::instance.IsAnimating())
      {
         state = State::Idle;
      }
      break;
   }
}


void StateMachine::ServiceIdle()
{
   if (CheckAcesToMove())
      return;

   switch (a2::getchar())
   {
   case 'N':
      // new game...
      Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());
      drawing1.DrawBackground();
      drawing1.DrawGame();
      a2::PAGE2OFF();
      break;

   case 'M':
      // new game...
      Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());
      drawing2.DrawBackground();
      drawing2.DrawGame();
      a2::PAGE2ON();
      break;

   default:
      break;
   }
}


/// \brief
///   Checks if there are any aces to move and starts the movement if so
/// \return
///   true if a card is being moved to the aces
///
bool StateMachine::CheckAcesToMove()
{
   // find the location of the card to move
   CardLocation startLocation = Game::instance.GetCardToMoveToAce();
   if (startLocation.IsNull())
      return false;

   // get the card
   Card card = Game::instance.GetCard(startLocation);

   // start the animation
   CardAnimator::instance.StartAnimation(card, startLocation, CardLocation::AcePile(card.GetSuit()));
   state = State::Animating;
   return true;
}

