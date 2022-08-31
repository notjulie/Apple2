
#ifndef STATEMACHINE_H
#define STATEMACHINE_H


class StateMachine {
public:
   constexpr StateMachine() {}

   void Service();

private:
   bool CheckAcesToMove();
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


#endif // STATEMACHINE_H
