// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_GAME_H_
#define SEAHAVEN2E_GAME_H_

#include "Card.h"
#include "CardLocation.h"


/** \brief
 * Represents the game as a whole
 */
class Game {
public:
   void Shuffle16(uint16_t instruction);

   Card GetCard(CardLocation location) const;
   void SetCard(CardLocation location, Card card);
   void RemoveCard(CardLocation location);

   Rank GetAcePileRank(uint8_t suitOrdinal) const { return acePiles[suitOrdinal]; }
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

   bool IsBottomOfColumn(CardLocation location) const;

public:
  static Game instance;

private:
   bool CanMoveToAce(Card card) const;
   void RemoveColumnCard(uint8_t column, uint8_t row);
   void SetColumnCard(uint8_t column, uint8_t row, Card card);
   void Shuffle8(uint8_t instruction);

private:
   Card deck[52];
   Rank acePiles[4];
   Card towers[4];
   uint8_t columnCounts[10];
   Card columnCards[50];
};

#endif  // SEAHAVEN2E_GAME_H_
