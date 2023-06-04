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
  // grab the bottom card from column 4
  CardLocation location;

  // grab the bottom card from the column nearest the center
  for (int i=0; i<5; ++i) {
    location = Game::instance.GetBottomColumnCardLocation(4 - i);
    if (!location.IsNull())
      break;
    location = Game::instance.GetBottomColumnCardLocation(5 + i);
    if (!location.IsNull())
      break;
  }

  // if we found none, find a tower
  if (location.IsNull()) {
    for (int i=0; i<4; ++i) {
      if (!Game::instance.GetTower(i).IsNull()) {
        location = CardLocation::Tower(i);
        break;
      }
    }
  }

  // set it
  SetLocation(location);
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
  if (location.IsNull())
    return;
  lastToggleTime = a2::VBLCounter::GetCounter();
  drawing1.ToggleCursor(location.GetX(), location.GetY());
}
