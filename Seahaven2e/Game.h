// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_GAME_H_
#define SEAHAVEN2E_GAME_H_

#include <C6502/Int6502.h>
#include "Card.h"
#include "CardLocation.h"
#include "SHAssert.h"

template <typename T, uint8_t Length> class SafeArray {
public:
   // default constructor does no initialization
   SafeArray() {}

   T &operator[](uint8_t index) {
      assert(index < Length);
      return array[index];
   }
   const T &operator[](uint8_t index) const {
      assert(index < Length);
      return array[index];
   }

private:
   T array[Length];
};


/// <summary>
/// Encapsulation of the ace piles... there are common operations that it's
/// worth capturing here so that I can more easily see where the optimizations
/// can be done.
/// </summary>
class AcePiles {
public:
   void Clear() { ranks[0] = ranks[1] = ranks[2] = ranks[3] = Rank::Null; }
   Card GetCard(CardLocation location) const;
   Rank GetRank(uint8_t suitOrdinal) const { return ranks[suitOrdinal]; }
   Rank GetRank(Suit suit) const { return ranks[suit.GetOrdinal()]; }
   CardLocation GetCardLocation(Card card) const;
   void RemoveCard(CardLocation location) { --ranks[location.GetAceSuitOrdinal()]; }
   void Set(CardLocation location, Card card) { ranks[location.GetAceSuitOrdinal()] = card.GetRank(); }

private:
   Rank ranks[4];
};

/** \brief
 * Represents the game as a whole
 */
class Game {
public:
   void Shuffle16(c6502::Int16 instruction);

   Card GetCard(CardLocation location) const;
   void SetCard(CardLocation location, Card card);
   void RemoveCard(CardLocation location);

   Rank GetAcePileRank(uint8_t suitOrdinal) const { return acePiles.GetRank(suitOrdinal); }
   inline Card GetTower(uint8_t index) const { return towers[index]; }

   CardLocation GetCardToMoveToAce() const;
   CardLocation GetBottomColumnCardLocation(uint8_t column) const;
   CardLocation GetCardLocation(Card card);
   CardLocation GetClosestOpenTowerToColumn(uint8_t column) const;
   CardLocation GetClosestOpenColumnToColumn(uint8_t column) const;
   CardLocation GetClosestOpenColumnToTower(uint8_t tower) const;
   Card GetTowerCard(uint8_t tower);

   // column card access
   uint8_t GetNumberOfCardsOnColumn(uint8_t column) const;
   Card GetColumnCard(uint8_t column, uint8_t row) const;
   uint8_t GetSizeOfMoveToColumnGroup(CardLocation location) const;
   uint8_t GetNumberOfAvailableTowers() const;

   CardLocation GetMoveToColumnDestination(CardLocation startLocation);
   bool IsBottomOfColumn(CardLocation location) const;

private:
   bool CanMoveToAce(CardLocation location) const;
   void RemoveColumnCard(uint8_t column, uint8_t row);
   void SetColumnCard(uint8_t column, uint8_t row, Card card);
   void Shuffle8(uint8_t instruction);

private:
   AcePiles acePiles;
   SafeArray<Card,4> towers;
   SafeArray<uint8_t,10> columnCounts;
   SafeArray<Card,50> columnCards;
};

#endif  // SEAHAVEN2E_GAME_H_
