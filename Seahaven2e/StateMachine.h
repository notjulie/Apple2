// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_STATEMACHINE_H_
#define SEAHAVEN2E_STATEMACHINE_H_

#include "Card.h"
#include "CardLocation.h"

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
   void Redo();
   void Undo();

private:
   enum class State {
      Uninitialized,
      Idle,
      Animating
   };

private:
   State state = State::Uninitialized;
};


#endif  // SEAHAVEN2E_STATEMACHINE_H_
