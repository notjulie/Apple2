// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_PERSISTENTSTATE_H_
#define SEAHAVEN2E_PERSISTENTSTATE_H_

#include <stdint.h>
#include <Apple2Lib/DOS.h>
#include "Game.h"
#include "UndoJournal.h"


/// <summary>
/// The data that we save
/// </summary>
class PersistentState {
public:
   void Load();
   bool CheckIntegrity() const;
   void Reset();

   uint16_t GetCurrentGameSeed() { return gameSeed; }
   uint16_t GetNextGameSeed() { return ++gameSeed; }

private:
   uint16_t CalculateChecksum() const;

// =============================
// ==== static data members ====
// =============================
public:
   static PersistentState instance;

// ============================================================
// ==== instance data; this is our stored image, so update ====
// ==== the version number in our data file name anytime   ====
// ==== anything changes                                   ====
// ============================================================
private:
   uint8_t signature1;
public:
   Game Game;
   UndoJournalPersist UndoJournal;
private:
   uint16_t gameSeed;
   uint8_t signature2;
   uint16_t checksum;
};

#endif  // SEAHAVEN2E_PERSISTENTSTATE_H_
