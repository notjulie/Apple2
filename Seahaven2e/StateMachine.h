// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_STATEMACHINE_H_
#define SEAHAVEN2E_STATEMACHINE_H_

#include "Card.h"
#include "CardLocation.h"
#include "UndoJournal.h"

/// <summary>
/// The top level state machine for the game
/// </summary>
class StateMachine {
public:
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
   void ServiceIdle();
   void StartCurrentGame();

   // card movement
   void MoveCard(Card card, CardLocation location);
   void StartNextMoveToTower();

private:
   enum class State {
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
