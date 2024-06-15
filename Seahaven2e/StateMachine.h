// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_STATEMACHINE_H_
#define SEAHAVEN2E_STATEMACHINE_H_

#include <Apple2Lib/Keyboard.h>
#include "Card.h"
#include "CardLocation.h"
#include "UndoJournal.h"

/// <summary>
/// The top level state machine for the game
/// </summary>
class StateMachine {
public:
   StateMachine() {}

   void Service();
   bool IsWriteRequested() const { return writeRequested; }
   void ClearWriteRequest() { writeRequested = false; }

private:
   // user commands
   void MoveToColumn();
   void MoveToTower();
   void NewGame();
   void BeginRedo(bool firstInGroup);
   void BeginUndo(bool firstInGroup);

   // state machine
   bool CheckAcesToMove();
   void EnterIdle();
   void EnterScreensave();
   void ExitScreensave();
   void Restart();
   void ServiceAnimations();
   void ServiceIdle();
   void StartCurrentGame();
   void ProcessInputKey(a2::KeyCode keyCode);

   // card movement
   void MoveSingleCard(CardLocation startLocation, CardLocation location);
   void MoveMultipleCards(CardLocation startLocation, CardLocation location, uint8_t count);
   void StartNextMoveToTower();

   // debug
#ifdef DEBUG
   void NewGameWithFullColumn();
#endif // DEBUG

private:
   enum class State : uint8_t {
      Uninitialized,
      Idle,
      Animating,
      Undoing,
      Redoing,
      MovingToTower,
      Screensave
   };

private:
   State state = State::Uninitialized;
   bool writeRequested = false;

   int8_t moveToTowerColumn;
   int8_t moveToTowerCurrentRow;
   int8_t moveToTowerEndRow;

   uint8_t idleEntryTime;
};


#endif  // SEAHAVEN2E_STATEMACHINE_H_
