// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_GAME_H_
#define SEAHAVEN2E_GAME_H_


#include <C6502/Int6502.h>
#include "Card.h"
#include "CardLocation.h"
#include "Deck.h"
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
   Rank GetRank(SuitOrdinal suitOrdinal) const { return ranks[(uint8_t)suitOrdinal]; }
   void RemoveCard(CardLocation location) { --ranks[location.GetAceSuitOrdinal()]; }
   void Set(CardLocation location, Card card) { ranks[location.GetAceSuitOrdinal()] = card.GetRank(); }

private:
   Rank ranks[4];
};

/** \brief
 * Represents the game as a whole; for optimization it is implemented
 * with all public methods as static, using the instance in the PersistentState
 * class.
 */
class Game {
public:
   static void Shuffle16(c6502::Int16 instruction);

   static Card GetCard(CardLocation location);
   static void SetCard(CardLocation location, Card card);
   static void RemoveCard(CardLocation location);

   static Rank GetAcePileRank(SuitOrdinal suitOrdinal);
   static Card GetTower(uint8_t index);

   static CardLocation GetCardToMoveToAce();
   static CardLocation GetBottomColumnCardLocation(uint8_t column);
   static CardLocation GetCardLocation(Card card);
   static CardLocation GetClosestOpenTowerToColumn(uint8_t column);
   static CardLocation GetClosestOpenColumnToColumn(uint8_t column);
   static CardLocation GetClosestOpenColumnToTower(uint8_t tower);
   static Card GetTowerCard(uint8_t tower);

   // column card access
   static uint8_t GetNumberOfCardsOnColumn(uint8_t column);
   static Card GetColumnCard(uint8_t column, uint8_t row);
   static uint8_t GetSizeOfMoveToColumnGroup(CardLocation location);
   static uint8_t GetNumberOfAvailableTowers();

   static CardLocation GetMoveToColumnDestination(CardLocation startLocation);

#ifdef DEBUG
   void NewGameWithFullColumn();
#endif // DEBUG

private:
   bool CanMoveToAce(CardLocation location) const;
   void RemoveColumnCard(uint8_t column, uint8_t row);
   void SetColumnCard(uint8_t column, uint8_t row, Card card);
   void Shuffle8(uint8_t instruction);
   bool IsBottomOfColumn(CardLocation location) const;

private:
   AcePiles acePiles;
   SafeArray<uint8_t,10> columnCounts;
   Deck deck;
};

#endif  // SEAHAVEN2E_GAME_H_
