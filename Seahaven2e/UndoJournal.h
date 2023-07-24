
#ifndef UNDOJOURNAL_H
#define UNDOJOURNAL_H

#include "Card.h"
#include "CardLocation.h"

class UndoJournal {
public:
   void LogMove(CompactCard card, CardLocation location) {}
};

#endif // UNDOJOURNAL_H
