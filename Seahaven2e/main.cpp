
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>

#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "Sprites.h"



/** \brief
 * Main entry point
 */
extern "C" int main()
{
   // call initializers
   Sprites::Initialize();
   PersistentState::instance = PersistentState();

   // set HGR
   a2::HIRESON();
   a2::TEXTOFF();
   a2::MIXEDON();

   // new game
   Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());

   // dump to the HGR screen
   DrawBackground();
   DrawGame();

   // enter main loop
   for (;;)
   {
      switch (a2::getchar())
      {
      case 'N':
         // new game...
         Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());
         DrawBackground();
         DrawGame();
         break;

      default:
         break;
      }
   }
}

