
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>

#include "Drawing.h"
#include "Game.h"
#include "Sprites.h"



/** \brief
 * Main entry point
 */
extern "C" int main()
{
   // call initializes
   Sprites::Initialize();

   // set HGR
   a2::HIRESON();
   a2::TEXTOFF();

   // new game
   Game::instance.Initialize();

   // dump to the HGR screen
   DrawBackground();
   //DrawABunchOfCards();
   //DrawTest();
   //DrawSprites();
   DrawGame();
}

