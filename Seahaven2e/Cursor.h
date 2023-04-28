// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CURSOR_H_
#define SEAHAVEN2E_CURSOR_H_

#include "CardLocation.h"

class Cursor {
 public:
  void Start(CardLocation location);

 public:
  static Cursor instance;
};

#endif  // SEAHAVEN2E_CURSOR_H_
