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

private:
   // user commands
   void MoveToColumn();
   void MoveToTower();
   void NewGame();
   void BeginRedo();
   void BeginUndo();

   // state machine
   bool CheckAcesToMove();
   void EnterIdle();
   void ServiceIdle();
   void RedoNext();
   void UndoNext();

   // card movement
   void MoveCard(CompactCard card, CardLocation location);

private:
   enum class State {
      Uninitialized,
      Idle,
      Animating,
      Undoing,
      Redoing
   };

private:
   State state = State::Uninitialized;
   UndoGroupID currentUndoGroup;
};


#endif  // SEAHAVEN2E_STATEMACHINE_H_
