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
   Cursor() = default;

   void CursorHasBeenObliterated();
   void Hide();
   void SetCursorLocationToDefault();
   void Show();
   void Service();

   void Up();
   void Down();
   void Left();
   void Right();

   CardLocation GetLocation() const;

public:
   static Cursor instance;

private:
   enum class State {
      Idle,
      On,
      Off
   };

private:
   void AdjustColumn();
   CardLocation GetClosestCardTo(CardLocation start);
   CardLocation GetClosestColumnCardTo(CardLocation start);
   CardLocation GetClosestTowerCardTo(CardLocation start);
   CardLocation GetClosestCardOnColumn(uint8_t column, uint8_t startIndex);
   void Toggle();
   void UpdateDisplayLocation();

private:
   // state must be initialized by constructor
   State state = State::Idle;

   // state variables that only matter in non-idle states
   uint8_t lastToggleTime;
   CardLocation currentDisplayLocation;
   uint8_t gridRow;
   uint8_t gridColumn;
};

#endif  // SEAHAVEN2E_CURSOR_H_
