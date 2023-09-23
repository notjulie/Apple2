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


void UndoJournal::Clear()
{
   assert(0);
   // clear the persist data
   PersistentState::instance.UndoJournal.Clear();
}


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
   assert(data.entryCount < UndoJournalPersist::JournalMaxLength);

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


__attribute__((noinline)) void UndoJournal::StartNewUndo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // truncate anything in the list beyond the current position
   data.entryCount = data.currentPosition;

   // set the current undo group to something other than the group
   // of the last entry
   if (data.entryCount > 0)
   {
      currentUndoGroup = (UndoGroupID)((((uint8_t)data.cards[data.entryCount - 1].GetGroupID()) + 1) & 3);
   }
   else
   {
      currentUndoGroup = UndoGroupID::Group0;
   }
}


UndoInstruction UndoJournal::GetFirstRedo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // get the group of the next item
   if (data.currentPosition >= data.entryCount)
      return UndoInstruction::Null();
   currentUndoGroup = data.cards[data.currentPosition].GetGroupID();

   // return
   return GetNextRedo();
}

UndoInstruction UndoJournal::GetFirstUndo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // get the group of the next item
   if (data.currentPosition == 0)
      return UndoInstruction::Null();
   currentUndoGroup = data.cards[data.currentPosition - 1].GetGroupID();

   // return
   return GetNextUndo();
}


UndoInstruction UndoJournal::GetNextRedo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // check the group of the next item
   if (data.currentPosition >= data.entryCount)
      return UndoInstruction::Null();
   if (currentUndoGroup != data.cards[data.currentPosition].GetGroupID())
      return UndoInstruction::Null();

   UndoInstruction result = data.PeekRedo();
   ++data.currentPosition;
   return result;
}


UndoInstruction UndoJournal::GetNextUndo()
{
   auto &data = PersistentState::instance.UndoJournal;

   // check the group of the next item
   if (data.currentPosition == 0)
      return UndoInstruction::Null();
   if (currentUndoGroup != data.cards[data.currentPosition - 1].GetGroupID())
      return UndoInstruction::Null();

   UndoInstruction result = data.PeekUndo();
   --data.currentPosition;
   return result;
}


// ======================================================
// ======================================================
//   class UndoJournalPersist
// ======================================================
// ======================================================

void UndoJournalPersist::Clear()
{
   entryCount = 0;
   currentPosition = 0;
}

/// <summary>
/// Returns a look at the next move to undo without changing the journal's state;
/// returns Null() if there is nothing to undo.
/// </summary>
UndoInstruction UndoJournalPersist::PeekUndo() const
{
   auto &game = PersistentState::instance.Game;

   UndoInstruction result = UndoInstruction::Null();

   // make sure we have something from the group
   if (currentPosition == 0)
      return result;

   // get the card to move
   uint8_t position = currentPosition - 1;
   result.card = cards[position].GetCard();

   // get its current location
   CardLocation currentLocation = game.GetCardLocation(result.card);
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
   auto &game = PersistentState::instance.Game;

   UndoInstruction result = UndoInstruction::Null();

   // see if we have something to redo
   if (currentPosition >= entryCount)
      return result;

   // get the card to move
   result.card = cards[currentPosition].GetCard();

   // get its current location
   CardLocation currentLocation = game.GetCardLocation(result.card);
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
