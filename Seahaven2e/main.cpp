// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>

#include "CardAnimator.h"
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "SHAssert.h"
#include "Sprites.h"
#include "StateMachine.h"

static StateMachine stateMachine;


/// <summary>
/// Main entry point
/// </summary>
extern "C" int main()
{
   // call initializers
   Sprites::Initialize();
   PersistentState::instance = PersistentState();
   stateMachine = StateMachine();
   CardAnimator::instance.Initialize();
   Cursor::instance = Cursor();

   // enter main loop
   for (;;)
   {
     // update the VBL counter
     a2::VBLCounter::Update();

     // give the state machine its timeslice
     stateMachine.Service();
   }
}


/// <summary>
/// Alternate entry point that just cleans up the screen and exits;
/// I can use this in AppleWin to break into the BASIC command line
/// so that I can BSAVE to a disk image.  Basically, I just code a
/// JMP to this at $6004 so that if I pop into the AppleWin debugger
/// I can jump here, and it will clean up the screen and take me to
/// the BASIC prompt, at which point I can use DOS commands to save
/// the binary to a disk.  It's a little Rube-Goldberg.  And
/// therefore awesome.
/// </summary>
extern "C" void exit()
{
   a2::PAGE2OFF();
   a2::TEXTON();
   a2::DOSWRM();
}
