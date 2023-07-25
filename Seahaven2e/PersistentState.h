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

   // undo related
   void LogMove(CompactCard card, CardLocation startLocation, CardLocation endLocation) { undoJournal.LogMove(card, startLocation, endLocation); }
   bool PopUndo(CompactCard &card, CardLocation &location) { return undoJournal.PopUndo(card, location); }

public:
   static PersistentState instance;

private:
   uint16_t gameSeed = 0;
   UndoJournal undoJournal;
};

#endif  // SEAHAVEN2E_PERSISTENTSTATE_H_
