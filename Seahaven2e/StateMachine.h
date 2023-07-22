// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_STATEMACHINE_H_
#define SEAHAVEN2E_STATEMACHINE_H_


/// <summary>
/// The top level state machine for the game
/// </summary>
class StateMachine {
 public:
  constexpr StateMachine() {}

  void Service();

 private:
  bool CheckAcesToMove();
  void NewGame();
  void EnterIdle();
  void MoveToColumn();
  void MoveToTower();
  void ServiceIdle();

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
