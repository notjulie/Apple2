// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#ifndef UNDOJOURNAL_H
#define UNDOJOURNAL_H

#include "Card.h"
#include "CardLocation.h"

struct UndoInstruction
{
   CompactCard card;
   CardLocation location;
};

class UndoJournal {
public:
   void LogMove(CompactCard card, CardLocation startLocation, CardLocation endLocation);
   UndoInstruction PopRedo();
   UndoInstruction PopUndo();

private:
   static constexpr uint8_t JournalMaxLength = 150;

private:
   uint8_t entryCount = 0;
   uint8_t currentPosition = 0;
   CompactCard cards[JournalMaxLength];
   uint8_t locations[JournalMaxLength];
};

#endif // UNDOJOURNAL_H
