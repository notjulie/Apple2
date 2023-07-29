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
   bool CheckAcesToMove();
   void EnterIdle();
   void MoveCard(CompactCard card, CardLocation location);
   void MoveToColumn();
   void MoveToTower();
   void NewGame();
   void ServiceIdle();
   void BeginRedo();
   void BeginUndo();
   void RedoNext();
   void UndoNext();

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
