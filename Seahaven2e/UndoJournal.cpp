// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "UndoJournal.h"
#include "Game.h"
#include "SHAssert.h"


/// <summary>
/// Logs the movement to the journal
/// </summary>
void UndoJournal::LogMove(UndoGroupID groupID, CompactCard card, CardLocation startLocation, CardLocation endLocation)
{
   // we can't redo beyond the current point... truncate the journal
   // at the current position
   entryCount = currentPosition;

   // TODO: if the journal fills up (because some doofus keeps moving
   // cards back and forth) we'll have to delete items off the front of
   // the journal
   assert(entryCount < JournalMaxLength);

   // append
   CardAndGroup cardAndGroup;
   cardAndGroup.SetCard(card);
   cardAndGroup.SetGroup(groupID);
   cards[entryCount] = cardAndGroup;

   // XOR the start location and the end location... then we can always
   // get one if we know the other
   locations[entryCount] = startLocation.AsUint8() ^ endLocation.AsUint8();

   // increment
   ++entryCount;
   currentPosition = entryCount;
}


/// <summary>
/// Returns a look at the next move to undo without changing the journal's state;
/// returns Null() if there is nothing to undo.
/// </summary>
UndoInstruction UndoJournal::PeekUndo() const
{
   UndoInstruction result = UndoInstruction::Null();

   // make sure we have something from the group
   if (currentPosition == 0)
      return result;

   // get the card to move
   uint8_t position = currentPosition - 1;
   result.cardAndGroup = cards[position];
   CompactCard card = result.GetCard();

   // get its current location
   CardLocation currentLocation = Game::instance.GetCardLocation(card);
   assert(!currentLocation.IsNull());

   // get the target location; given the current location we get that by XORing
   // that location with the stored location integers
   result.location = CardLocation::FromUint8(locations[position] ^ currentLocation.AsUint8());
   return result;
}


/// <summary>
/// Pulls a move to redo off of the journal; returns false if there
/// was nothing to redo.
/// </summary>
UndoInstruction UndoJournal::PeekRedo() const
{
   UndoInstruction result = UndoInstruction::Null();

   // see if we have something to redo
   if (currentPosition >= entryCount)
      return result;

   // get the card to move
   result.cardAndGroup = cards[currentPosition];

   // get its current location
   result.cardAndGroup = cards[currentPosition];
   CardLocation currentLocation = Game::instance.GetCardLocation(result.GetCard());
   assert(!currentLocation.IsNull());

   // get the target location; given the current location we get that by XORing
   // that location with the stored location integers
   result.location = CardLocation::FromUint8(locations[currentPosition] ^ currentLocation.AsUint8());
   return result;
}


void UndoJournal::PopRedo()
{
   ++currentPosition;
}

void UndoJournal::PopUndo()
{
   --currentPosition;
}

UndoGroupID UndoJournal::StartNewUndo()
{
   // truncate anything in the list beyond the current position
   entryCount = currentPosition;

   // peek an undo
   UndoInstruction undo = PeekUndo();

   // all we need to do is make sure that we return a group ID
   // that's different from the last undo
   return (UndoGroupID)((uint8_t)undo.GetGroup() + 1);
}





bool UndoInstruction::IsNull() const
{
   return location.IsNull();
}

UndoInstruction UndoInstruction::Null()
{
   UndoInstruction result;
   result.location = CardLocation::Null();
   return result;
}
