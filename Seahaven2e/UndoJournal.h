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
   CompactCard GetCard() const { return CompactCard::FromOrdinal(cardOrdinal); }
   UndoGroupID GetGroupID() const { return (UndoGroupID)group; }

   void SetCard(CompactCard card) { cardOrdinal = card.ToOrdinal(); }
   void SetGroup(UndoGroupID groupID) { group = (uint8_t)groupID; }

private:
   uint8_t cardOrdinal : 6;
   uint8_t group : 2;
};


/// <summary>
/// Encapsulation of an undo/redo step retrieved from the journal
/// </summary>
struct UndoInstruction
{
   CardAndGroup cardAndGroup;
   CardLocation location;

   bool IsNull() const;
   CompactCard GetCard() const { return cardAndGroup.GetCard(); }
   UndoGroupID GetGroup() const { return cardAndGroup.GetGroupID(); }

   static UndoInstruction Null();
};


/// <summary>
/// The journal of moves that can be undone/redone
/// </summary>
class UndoJournal {
public:
   UndoGroupID StartNewUndo();
   void LogMove(UndoGroupID groupID, CompactCard card, CardLocation startLocation, CardLocation endLocation);
   UndoInstruction PeekRedo() const;
   UndoInstruction PeekUndo() const;
   void PopRedo();
   void PopUndo();

private:
   static constexpr uint8_t JournalMaxLength = 150;

private:
   static_assert(sizeof(CardAndGroup) == 1, "CardAndGroup is too large");

private:
   uint8_t entryCount = 0;
   uint8_t currentPosition = 0;
   CardAndGroup cards[JournalMaxLength];
   uint8_t locations[JournalMaxLength];
};

#endif // UNDOJOURNAL_H
