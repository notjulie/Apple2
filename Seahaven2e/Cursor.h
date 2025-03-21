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
   Cursor() {}
   Cursor(const Cursor &) = delete;
   Cursor &operator=(const Cursor &) = delete;
   void Initialize();

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
   enum class State : uint8_t {
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
   State state;

   // state variables that only matter in non-idle states
   uint8_t lastToggleTime;
   CardLocation currentDisplayLocation;
   uint8_t gridRow;
   int8_t gridColumn;
};

#endif  // SEAHAVEN2E_CURSOR_H_
