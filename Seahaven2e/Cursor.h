// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CURSOR_H_
#define SEAHAVEN2E_CURSOR_H_

#include "CardLocation.h"

/// <summary>
/// Cursor implementation
/// </summary>
class Cursor {
 public:
  void SetCursorLocationToDefault();
  void Show();
  void Service();

  void Up() {}
  void Down() {}
  void Left() {}
  void Right() {}

 public:
  static Cursor instance;

 private:
  enum class State {
    Idle,
    On,
    Off
  };

 private:
  void SetLocation(CardLocation location);
  void Toggle();

 private:
  uint8_t lastToggleTime = 0;
  State state = State::Idle;
  CardLocation location;
};

#endif  // SEAHAVEN2E_CURSOR_H_
