// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/VBLCounter.h>

#include "CardAnimator.h"
#include "Cursor.h"
#include "PersistentState.h"
#include "SHAssert.h"
#include "Sprites.h"
#include "StateMachine.h"
#include "TextScreenManager.h"


static StateMachine stateMachine;


/// <summary>
/// Main entry point
/// </summary>
extern "C" int main()
{
   // show the splash screen while we load the PersistentState
   TextScreenManager::instance.Initialize();
   TextScreenManager::instance.ShowScreen(TextScreenID::Splash);

   // load the persistent state; note that this does a DOS call and
   // needs to be done here rather than higher in the call stack in case
   // there are side effects on our zero-page data
   PersistentState::instance.Load();

   // initialize the CardAnimator first so that it hides the screen
   // output from calls to DOS below
   CardAnimator::instance.Initialize();

   // check the integrity
   if (!PersistentState::instance.CheckIntegrity())
      PersistentState::instance.Reset();

   // call initializers
   stateMachine = StateMachine();
   Cursor::instance.Initialize();

   // enter main loop
   for (;;)
   {
     // update the VBL counter
     a2::VBLCounter::Update();

     // give the state machine its timeslice
     stateMachine.Service();

     // check for write requests
     if (stateMachine.IsWriteRequested())
     {
        PersistentState::instance.Save();
        stateMachine.ClearWriteRequest();
     }
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
extern "C" [[noreturn]] void exit()
{
   a2::PAGE2OFF();
   a2::TEXTON();
   a2::DOSWRM();
}
