// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Cursor.h"

#include <Apple2Lib/VBLCounter.h>
#include "CardAnimator.h"
#include "Drawing.h"
#include "PersistentState.h"
#include "SHAssert.h"


/// <summary>
/// Our global instance
/// </summary>
Cursor Cursor::instance;


void Cursor::Initialize()
{
   state = State::Idle;
}

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
__attribute__((noinline)) void Cursor::SetCursorLocationToDefault()
{
   gridRow = 255;
   gridColumn = 4;
   AdjustColumn();
}


/// <summary>
/// Moves the cursor up
/// </summary>
__attribute__((noinline)) void Cursor::Up()
{
   // get the current location
   CardLocation location = GetLocation();

   // update the gridRow
   gridRow = location.GetGridRow();
   if (gridRow > 0)
      --gridRow;

   // update the display
   UpdateDisplayLocation();
}


/// <summary>
/// Moves the cursor down
/// </summary>
__attribute__((noinline)) void Cursor::Down()
{
   // get the current location
   CardLocation location = GetLocation();

   // update the gridRow
   gridRow = location.GetGridRow();
   if (gridRow < 255)
      ++gridRow;

   // if that didn't change the location then set the row to the
   // max so that we just ride along the bottom as we cursor left and
   // right
   if (location == GetLocation())
      gridRow = 255;

   // update the display
   UpdateDisplayLocation();
}


/// <summary>
/// Moves the cursor left
/// </summary>
__attribute__((noinline)) void Cursor::Left()
{
   // remember where we started in case we don't find anything
   int8_t startColumn = gridColumn;

   // move left
   while (--gridColumn >= 0)
   {
      // if we have a valid location accept it as the new location
      if (!GetLocation().IsNull())
      {
         UpdateDisplayLocation();
         return;
      }
   }

   // didn't find anything... restore the original column
   gridColumn = startColumn;
}



/// <summary>
/// Moves the cursor right
/// </summary>
__attribute__((noinline)) void Cursor::Right()
{
   // remember where we started in case we don't find anything
   int8_t startColumn = gridColumn;

   // move right
   while (++gridColumn <= 9)
   {
      // if we have a valid location accept it as the new location
      if (!GetLocation().IsNull())
      {
         UpdateDisplayLocation();
         return;
      }
   }

   // didn't find anything... restore the original column
   gridColumn = startColumn;
}


/// <summary>
/// Hides the cursor
/// </summary>
__attribute__((noinline)) void Cursor::Hide()
{
   if (state == State::On)
   {
      Toggle();
   }
   state = State::Idle;
}


/// <summary>
/// Shows the cursor at its current location... if the current location does not
/// have a card on it we shuffle the cursor to a spot that does
/// </summary>
__attribute__((noinline)) void Cursor::Show()
{
   if (state == State::Idle)
   {
      // make sure that we're on a column/tower with cards on it
      AdjustColumn();

      // get the location
      currentDisplayLocation = GetLocation();

      // turn it on
      Toggle();
      state = State::On;
   }
}


/// <summary>
/// Performs periodic actions
/// </summary>
__attribute__((noinline)) void Cursor::Service() {
  uint8_t sinceLastToggle = a2::VBLCounter::GetCounter().lo - lastToggleTime;

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
__attribute__((noinline)) void Cursor::Toggle()
{
   lastToggleTime = a2::VBLCounter::GetCounter().lo;
   if (!currentDisplayLocation.IsNull())
   {
      CardAnimator::instance.GetOnscreenPage().GetDrawing().ToggleCursor(currentDisplayLocation);
   }
}


/// <summary>
/// Gets the location of the card closest to the given location
/// </summary>
CardLocation Cursor::GetClosestCardTo(CardLocation start)
{
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
CardLocation Cursor::GetClosestCardOnColumn(uint8_t column, uint8_t startIndex)
{
   if (column < 10)
   {
      CardLocation bottomCard = Game::GetBottomColumnCardLocation(column);

     if (!bottomCard.IsNull())
     {
        if (bottomCard.GetRow() < startIndex)
        {
           return bottomCard;
        }
        else
        {
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

   for (uint8_t i=0; i<4; ++i)
   {
      uint8_t tower = startTower + i;
      Card card = Game::GetTowerCard(tower);
      if (!card.IsNull())
         return CardLocation::Tower(tower);

      tower = startTower - i;
      card = Game::GetTowerCard(tower);
      if (!card.IsNull())
         return CardLocation::Tower(tower);
   }

   return CardLocation::Null();
}


/// <summary>
/// Moves the cursor to the nearest column with something on it
/// if the current column is empty
/// </summary>
void Cursor::AdjustColumn()
{
   uint8_t startColumn = gridColumn;

   // adjust the scan radius until we find something
   for (uint8_t i=0; i<9; ++i)
   {
      int8_t c = (int8_t)startColumn - i;
      if (c >= 0)
      {
         gridColumn = c;
         if (!GetLocation().IsNull())
            return;
      }

      c = startColumn  + i;
      if (c < 10)
      {
         gridColumn = c;
         if (!GetLocation().IsNull())
            return;
      }
   }
}


/// <summary>
/// Converts the current row/column setting to a card location
/// </summary>
CardLocation Cursor::GetLocation() const
{
   // see how many cards are on the column... that tells us the max row
   uint8_t maxRow = Game::GetNumberOfCardsOnColumn(gridColumn);
   uint8_t row = gridRow;
   if (row > maxRow)
      row = maxRow;

   // row zero is the tower, if there's one on this grid column
   if (row == 0)
   {
      int8_t tower = (int8_t)gridColumn - 3;
      if (tower>=0 && tower<4 && !Game::GetTower(tower).IsNull())
         return CardLocation::Tower(tower);
      if (maxRow == 0)
          return CardLocation::Null();
      row = 1;
   }

   return CardLocation::Column(gridColumn, row - 1);
}


/// <summary>
/// moves the cursor to the current location
/// </summary>
void Cursor::UpdateDisplayLocation()
{
   CardLocation location = GetLocation();
   switch (state)
   {
   case State::Idle:
      break;

   case State::Off:
      if (location != currentDisplayLocation)
      {
         currentDisplayLocation = GetLocation();
         Toggle();
         state = State::On;
      }
      break;

   case State::On:
      if (location != currentDisplayLocation)
      {
         Toggle();
         currentDisplayLocation = GetLocation();
         Toggle();
      }
      break;

   default:
      assert(0);
      break;
   }
}

