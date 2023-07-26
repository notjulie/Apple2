// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#ifndef UNDOJOURNAL_H
#define UNDOJOURNAL_H

#include "Card.h"
#include "CardLocation.h"

class UndoJournal {
public:
   void LogMove(CompactCard card, CardLocation startLocation, CardLocation endLocation);
   bool PopRedo(CompactCard &card, CardLocation &location);
   bool PopUndo(CompactCard &card, CardLocation &location);

private:
   static constexpr uint8_t JournalMaxLength = 150;

private:
   uint8_t entryCount = 0;
   uint8_t currentPosition = 0;
   CompactCard cards[JournalMaxLength];
   uint8_t locations[JournalMaxLength];
};

#endif // UNDOJOURNAL_H
