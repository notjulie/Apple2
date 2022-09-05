
#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>

#include "CardAnimator.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "Sprites.h"
#include "StateMachine.h"

static StateMachine stateMachine;


/** \brief
 * Main entry point
 */
extern "C" int main()
{
   // call initializers
   Sprites::Initialize();
   PersistentState::instance = PersistentState();
   stateMachine = StateMachine();
   CardAnimator::instance = CardAnimator();

   // enter main loop
   uint8_t lastCount = a2::VBLCounter::GetCounter();
   for (;;)
   {
      a2::VBLCounter::Update();
      uint8_t count = a2::VBLCounter::GetCounter();
      uint8_t elapsed = count - lastCount;
      if (elapsed >= 60)
      {
         lastCount = count;
         a2::puts("BLEEM");
      }

      stateMachine.Service();
   }
}

