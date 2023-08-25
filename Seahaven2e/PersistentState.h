// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_PERSISTENTSTATE_H_
#define SEAHAVEN2E_PERSISTENTSTATE_H_

#include <stdint.h>
#include "Game.h"
#include "UndoJournal.h"


class PersistentState {
public:
   PersistentState() {}

   uint16_t GetCurrentGameSeed() { return gameSeed; }
   uint16_t GetNextGameSeed() { return ++gameSeed; }

public:
   Game Game;
   UndoJournalPersist UndoJournal;

public:
   static PersistentState instance;

private:
   uint16_t gameSeed = 0;
};

#endif  // SEAHAVEN2E_PERSISTENTSTATE_H_
