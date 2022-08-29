
#include "StateMachine.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
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


bool StateMachine::CheckAcesToMove()
{
   CardLocation aceLocation = Game::instance.GetAceToMove();
   if (aceLocation.IsNull())
      return false;

   a2::PRBYTE(0x42);
   a2::MONITOR();
   return true;
}

