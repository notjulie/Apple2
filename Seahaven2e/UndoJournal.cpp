// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/ROM.h>
#include "UndoJournal.h"
#include "Game.h"
#include "SHAssert.h"


/// <summary>
/// Logs the movement to the journal
/// </summary>
void UndoJournal::LogMove(CompactCard card, CardLocation startLocation, CardLocation endLocation)
{
   // we can't redo beyond the current point... truncate the journal
   // at the current position
   entryCount = currentPosition;

   // TODO: if the journal fills up (because some doofus keeps moving
   // cards back and forth) we'll have to delete items off the front of
   // the journal
   assert(entryCount < JournalMaxLength);

   // append
   cards[entryCount] = card;

   // XOR the start location and the end location... then we can always
   // get one if we know the other
   locations[entryCount] = startLocation.AsUint8() ^ endLocation.AsUint8();

   // increment
   ++entryCount;
   currentPosition = entryCount;
}


/// <summary>
/// Pulls a move to undo off of the journal; returns false if there
/// was nothing to undo.
/// </summary>
bool UndoJournal::PopUndo(CompactCard &card, CardLocation &location)
{
   if (currentPosition == 0)
      return false;
   --currentPosition;

   card = cards[currentPosition];
   location = CardLocation::FromUint8(
      locations[currentPosition] ^ Game::instance.GetCardLocation(card).AsUint8()
      );
   return true;
}


/// <summary>
/// Pulls a move to redo off of the journal; returns false if there
/// was nothing to redo.
/// </summary>
bool UndoJournal::PopRedo(CompactCard &card, CardLocation &location)
{
   if (currentPosition >= entryCount)
      return false;

   card = cards[currentPosition];
   location = CardLocation::FromUint8(
      locations[currentPosition] ^ Game::instance.GetCardLocation(card).AsUint8()
      );

   ++currentPosition;
   return true;
}
