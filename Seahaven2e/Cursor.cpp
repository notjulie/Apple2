// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Cursor.h"

#include <Apple2Lib/VBLCounter.h>
#include "CardAnimator.h"
#include "Drawing.h"


/// <summary>
/// Our global instance
/// </summary>
Cursor Cursor::instance;


/// <summary>
/// Adjusts our state when the animator has overwritten the cursor
/// </summary>
void Cursor::CursorHasBeenObliterated()
{
   state = State::Idle;
}


/// <summary>
/// Sets the cursor location to the bottom card on column 4, adjusting
/// as needed if the column is empty
/// </summary>
void Cursor::SetCursorLocationToDefault() {
  // just set it to the bottomest of the bottom on column 4 and
  // let our cursor adjustment logic handle it from there
  CardLocation defaultLocation = CardLocation::Column(4, CardLocations::MaxColumnCards - 1);
  SetAndAdjustLocation(defaultLocation);
}


/// <summary>
/// Moves the cursor up
/// </summary>
void Cursor::Up()
{
  SetAndAdjustLocation(location.Up());
}


/// <summary>
/// Moves the cursor down
/// </summary>
void Cursor::Down()
{
  SetAndAdjustLocation(location.Down());
}


/// <summary>
/// Moves the cursor left
/// </summary>
void Cursor::Left()
{
  SetAndAdjustLocation(location.Left());
}


/// <summary>
/// Moves the cursor right
/// </summary>
void Cursor::Right()
{
  SetAndAdjustLocation(location.Right());
}


/// <summary>
/// Hides the cursor
/// </summary>
void Cursor::Hide()
{
   if (state == State::On)
   {
      Toggle();
      state = State::Idle;
   }
}


/// <summary>
/// Shows the cursor at its current location... if the current location does not
/// have a card on it we shuffle the cursor to a spot that does
/// </summary>
void Cursor::Show() {
  location = GetClosestCardTo(location);
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


/// <summary>
/// Toggles the cursor
/// </summary>
void Cursor::Toggle()
{
   if (location.IsNull())
      return;
   lastToggleTime = a2::VBLCounter::GetCounter();
   CardAnimator::instance.GetOnscreenPage()->GetDrawing()->ToggleCursor(location.GetX(), location.GetY());
}


/// <summary>
/// Gets the location of the card closest to the given location
/// </summary>
CardLocation Cursor::GetClosestCardTo(CardLocation start) {
  CardLocation result = CardLocation::Null();

  if (start.IsColumn()) {
    result = GetClosestColumnCardTo(start);
    if (result.IsNull()) {
      result = GetClosestTowerCardTo(start);
    }
  } else if (start.IsTower()) {
    result = GetClosestTowerCardTo(start);
    if (result.IsNull())
      result = GetClosestColumnCardTo(start);
  }

  return result;
}


/// <summary>
/// Gets the location of the column card closest to the given location
/// </summary>
CardLocation Cursor::GetClosestColumnCardTo(CardLocation start) {
  uint8_t startColumn;
  uint8_t startIndex;

  if (start.IsColumn()) {
    startColumn = start.GetColumn();
    startIndex = start.GetRow();
  } else if (start.IsTower()) {
    // start looking from the top of the column nearest the tower
    startColumn = start.GetTowerIndex() + 3;
    startIndex = 0;
  } else {
    return CardLocation::Null();
  }

  CardLocation result = CardLocation::Null();
  for (uint8_t i=0; i<10; ++i) {
    result = GetClosestCardOnColumn(startColumn + i, startIndex);
    if (!result.IsNull())
      break;
    result = GetClosestCardOnColumn(startColumn - i, startIndex);
    if (!result.IsNull())
      break;
  }

  return result;
}


/// <summary>
/// Gets the closest card to the given position on the given column
/// </summary>
CardLocation Cursor::GetClosestCardOnColumn(uint8_t column, uint8_t startIndex) {
  if (column < 10) {
    CardLocation bottomCard = Game::instance.GetBottomColumnCardLocation(column);

    if (!bottomCard.IsNull()) {
      if (bottomCard.GetRow() < startIndex) {
        return bottomCard;
      } else {
        return CardLocation::Column(column, startIndex);
      }
    }
  }

  return CardLocation::Null();
}


/// <summary>
/// Gets the location of the tower card closest to the given location
/// </summary>
CardLocation Cursor::GetClosestTowerCardTo(CardLocation start) {
  uint8_t startTower;

  if (start.IsColumn()) {
    startTower = start.GetColumn() - 3;
    if ((int8_t)startTower < 0)
      startTower = 0;
    if (startTower > 3)
      startTower = 3;
  } else if (start.IsTower()) {
    startTower = start.GetTowerIndex();
  } else {
    return CardLocation::Null();
  }

  for (uint8_t i=0; i<4; ++i) {
    uint8_t tower = startTower + i;
    CompactCard card = Game::instance.GetTowerCard(tower);
    if (!card.IsNull())
      return CardLocation::Tower(tower);

    tower = startTower - i;
    card = Game::instance.GetTowerCard(tower);
    if (!card.IsNull())
      return CardLocation::Tower(tower);
  }

  return CardLocation::Null();
}


void Cursor::SetAndAdjustLocation(CardLocation location) {
  SetLocation(GetClosestCardTo(location));
}
