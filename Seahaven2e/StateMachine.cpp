
#include "StateMachine.h"
#include <Apple2Lib/IO.h>
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"

void StateMachine::Service()
{
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

