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
   // if we can undo, pre-decrement to the new position
   if (currentPosition == 0)
      return false;
   --currentPosition;

   // get the card to move and its current location
   card = cards[currentPosition];
   CardLocation currentLocation = Game::instance.GetCardLocation(card);
   assert(!currentLocation.IsNull());

   // get the target location; given the current location we get that by XORing
   // that location with the stored location integers
   location = CardLocation::FromUint8(locations[currentPosition] ^ currentLocation.AsUint8());
   return true;
}


/// <summary>
/// Pulls a move to redo off of the journal; returns false if there
/// was nothing to redo.
/// </summary>
bool UndoJournal::PopRedo(CompactCard &card, CardLocation &location)
{
   // see if we have something to redo
   if (currentPosition >= entryCount)
      return false;

   // get the card to move and its current location
   card = cards[currentPosition];
   CardLocation currentLocation = Game::instance.GetCardLocation(card);
   assert(!currentLocation.IsNull());

   // get the target location; given the current location we get that by XORing
   // that location with the stored location integers
   location = CardLocation::FromUint8(locations[currentPosition] ^ currentLocation.AsUint8());

   // update the position and done
   ++currentPosition;
   return true;
}
