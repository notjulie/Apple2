// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_PERSISTENTSTATE_H_
#define SEAHAVEN2E_PERSISTENTSTATE_H_

#include <stdint.h>
#include "UndoJournal.h"


class PersistentState {
public:
   uint16_t GetNextGameSeed() { return ++gameSeed; }

public:
   UndoJournal UndoJournal;

public:
   static PersistentState instance;

private:
   uint16_t gameSeed = 0;
};

#endif  // SEAHAVEN2E_PERSISTENTSTATE_H_
