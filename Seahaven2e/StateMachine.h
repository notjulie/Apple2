
#ifndef STATEMACHINE_H
#define STATEMACHINE_H


class StateMachine {
public:
   constexpr StateMachine() {}

   void Service();

private:
   void ServiceIdle();

private:
   enum class State {
      Uninitialized,
      Idle
   };

private:
   State state = State::Uninitialized;
};


#endif // STATEMACHINE_H