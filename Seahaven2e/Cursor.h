// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CURSOR_H_
#define SEAHAVEN2E_CURSOR_H_

#include "CardLocation.h"

class Cursor {
 public:
  void Start(CardLocation location);
  void Service();

 public:
  static Cursor instance;

 private:
  enum class State {
    Idle,
    On,
    Off
  };

 private:
  void Toggle();

 private:
  uint8_t lastToggleTime = 0;
  State state = State::Idle;
  CardLocation location;
};

#endif  // SEAHAVEN2E_CURSOR_H_
