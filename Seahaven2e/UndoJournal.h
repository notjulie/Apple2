// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#ifndef UNDOJOURNAL_H
#define UNDOJOURNAL_H

#include "Card.h"
#include "CardLocation.h"


/// <summary>
/// An identifier stored with each entry in the undo journal to indicate
/// a group to which it belongs; consecutive items of the same group get
/// undone/redone as a group
/// </summary>
enum class UndoGroupID : int8_t {
   None = -1,
   Group0 = 0,
   Group1 = 1,
   Group2 = 2,
   Group3 = 3
};


/// <summary>
/// Bitfield packing of card ordinal and group ID into a single byte
/// </summary>
class CardAndGroup {
public:
   CardAndGroup() {}

   Card GetCard() const { return Card::FromOrdinal(cardOrdinal); }
   UndoGroupID GetGroupID() const { return (UndoGroupID)group; }

   void SetCard(Card card) { cardOrdinal = card.ToOrdinal(); }
   void SetGroup(UndoGroupID groupID) { group = (uint8_t)groupID; }

private:
   uint8_t cardOrdinal : 6;
   uint8_t group : 2;
};
static_assert(sizeof(CardAndGroup) == 1, "CardAndGroup is too large");


/// <summary>
/// Encapsulation of an undo/redo step retrieved from the journal
/// </summary>
struct UndoInstruction
{
   // on construction we only initialize what we require to be
   // initialized
   UndoInstruction() {}

   Card card;
   CardLocation location;

   bool IsNull() const;

   static UndoInstruction Null();
};


/// <summary>
/// The persistent data associated with the undo journal
/// </summary>
class UndoJournalPersist {
public:
   // on construction we only initialize what we require to be
   // initialized
   UndoJournalPersist() {}

private:
   UndoInstruction PeekRedo() const;
   UndoInstruction PeekUndo() const;

private:
   static constexpr uint8_t JournalMaxLength = 150;

private:
   uint8_t entryCount = 0;
   uint8_t currentPosition = 0;
   CardAndGroup cards[JournalMaxLength];
   uint8_t locations[JournalMaxLength];

   friend class UndoJournal;
};


/// <summary>
/// The journal of moves that can be undone/redone
/// </summary>
class UndoJournal {
public:
   UndoJournal() {}

   void Clear();
   void Restart();
   void StartNewUndo();
   void LogMove(Card card, CardLocation startLocation, CardLocation endLocation);
   UndoInstruction GetFirstRedo();
   UndoInstruction GetFirstUndo();
   UndoInstruction GetNextRedo();
   UndoInstruction GetNextUndo();

public:
   static UndoJournal instance;

private:
   UndoGroupID currentUndoGroup;
};

#endif // UNDOJOURNAL_H
