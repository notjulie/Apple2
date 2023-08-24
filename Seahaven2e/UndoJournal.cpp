// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "UndoJournal.h"

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "Game.h"
#include "PersistentState.h"
#include "SHAssert.h"


// ======================================================
// ======================================================
//   class UndoJournal
// ======================================================
// ======================================================

UndoJournal UndoJournal::instance;

/// <summary>
/// Logs the movement to the journal
/// </summary>
__attribute__((noinline)) void UndoJournal::LogMove(Card card, CardLocation startLocation, CardLocation endLocation)
{
   auto &data = PersistentState::instance.UndoJournal;

   // we don't log moves to the aces at the beginning of the game...
   // they aren't undoable
   if (endLocation.IsAce() && data.currentPosition==0)
      return;

   // we can't redo beyond the current point... truncate the journal
   // at the current position
   data.entryCount = data.currentPosition;

   // TODO: if the journal fills up (because some doofus keeps moving
   // cards back and forth) we'll have to delete items off the front of
   // the journal
   assert(data.entryCount < JournalMaxLength);

   // append
   CardAndGroup cardAndGroup;
   cardAndGroup.SetCard(card);
   cardAndGroup.SetGroup(currentUndoGroup);
   data.cards[data.entryCount] = cardAndGroup;

   // XOR the start location and the end location... then we can always
   // get one if we know the other
   data.locations[data.entryCount] = startLocation.AsUint8() ^ endLocation.AsUint8();

   // increment
   ++data.entryCount;
   data.currentPosition = data.entryCount;
}


void UndoJournal::Restart()
{
   PersistentState::instance.UndoJournal.currentPosition = 0;
}


void UndoJournal::PopRedo()
{
   ++PersistentState::instance.UndoJournal.currentPosition;
}

void UndoJournal::PopUndo()
{
   --PersistentState::instance.UndoJournal.currentPosition;
}


void UndoJournal::StartNewUndo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // truncate anything in the list beyond the current position
   data.entryCount = data.currentPosition;

   // peek an undo
   UndoInstruction undo = PeekUndo();

   // all we need to do is make sure that we return a group ID
   // that's different from the last undo
   currentUndoGroup = (UndoGroupID)((uint8_t)undo.GetGroup() + 1);
}


UndoInstruction UndoJournal::PeekUndo() const
{
   return PersistentState::instance.UndoJournal.PeekUndo();
}


UndoInstruction UndoJournal::PeekRedo() const
{
   return PersistentState::instance.UndoJournal.PeekRedo();
}


// ======================================================
// ======================================================
//   class UndoJournalPersist
// ======================================================
// ======================================================

/// <summary>
/// Returns a look at the next move to undo without changing the journal's state;
/// returns Null() if there is nothing to undo.
/// </summary>
UndoInstruction UndoJournalPersist::PeekUndo() const
{
   UndoInstruction result = UndoInstruction::Null();

   // make sure we have something from the group
   if (currentPosition == 0)
      return result;

   // get the card to move
   uint8_t position = currentPosition - 1;
   result.cardAndGroup = cards[position];
   Card card = result.GetCard();

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
UndoInstruction UndoJournalPersist::PeekRedo() const
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


// ======================================================
// ======================================================
//   class UndoInstruction
// ======================================================
// ======================================================


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
