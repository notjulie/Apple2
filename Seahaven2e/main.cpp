// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <stdint.h>
#include <Apple2Lib/HGR.h>
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include <C6502/Memory.h>

#include "CardAnimator.h"
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "Sprites.h"
#include "StateMachine.h"


/// <summary>
/// the global state machine; force it into .data so that it gets allocated to
/// the output file and thus initialized
/// </summary>
__declspec(allocate(".data")) static StateMachine stateMachine;


/** \brief
 * Main entry point
 */
extern "C" int main() {
  // call initializers
  Sprites::Initialize();

  // enter main loop
  for (;;) {
    // update the VBL counter
    a2::VBLCounter::Update();

    // give the state machine its timeslice
    stateMachine.Service();
  }
}
