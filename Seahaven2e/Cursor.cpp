// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Cursor.h"

#include <Apple2Lib/VBLCounter.h>
#include "Drawing.h"

Cursor Cursor::instance;

/// <summary>
/// Starts showing the cursor at the given location
/// </summary>
void Cursor::Start(CardLocation location)
{
  this->location = location;
  Toggle();
  state = State::On;
}

void Cursor::Service() {
  uint8_t sinceLastToggle = a2::VBLCounter::GetCounter() - lastToggleTime;

  switch (state) {
  case State::On:
    if (sinceLastToggle >= 30) {
      Toggle();
      state = State::Off;
    }
    break;

  case State::Off:
    if (sinceLastToggle >= 15) {
      Toggle();
      state = State::On;
    }
    break;

  case State::Idle:
    break;
  }
}

void Cursor::Toggle() {
  lastToggleTime = a2::VBLCounter::GetCounter();
  drawing1.ToggleCursor(location.GetX(), location.GetY());
}
