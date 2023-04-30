// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Cursor.h"

#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include "Drawing.h"

Cursor Cursor::instance;

/// <summary>
/// Sets the cursor location to the bottom card on column 4, adjusting
/// as needed if the column is empty
/// </summary>
void Cursor::SetCursorLocationToDefault() {
  // TODO(RER): adjust if column is empty
  SetLocation(Game::instance.GetBottomColumnCardLocation(4));
  a2::PRBYTE((uint8_t)location.GetArea());
  a2::puts(" ");
  a2::PRBYTE(location.GetIndex());
}


/// <summary>
/// Shows the cursor at its current location
/// </summary>
void Cursor::Show() {
  if (state == State::Idle) {
    Toggle();
    state = State::On;
  }
}

/// <summary>
/// Starts showing the cursor at the given location
/// </summary>
void Cursor::SetLocation(CardLocation location) {
  switch (state) {
  case State::Idle:
  case State::Off:
    this->location = location;
    break;

  case State::On:
    Toggle();
    this->location = location;
    Toggle();
    break;

  default:
    a2::puts("CURSORSETLOCATION FAIL");
    break;
  }
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
