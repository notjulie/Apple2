// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Cursor.h"

#include "Drawing.h"

Cursor Cursor::instance;


void Cursor::Start(CardLocation location)
{
  drawing1.ToggleCursor(location.GetX(), location.GetY());
}

