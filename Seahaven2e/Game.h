// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_GAME_H_
#define SEAHAVEN2E_GAME_H_

#include "Card.h"
#include "CardLocation.h"
#include "Column.h"


/** \brief
 * Represents the game as a whole
 */
class Game {
public:
   void Shuffle16(uint16_t instruction);

   CompactCard GetCard(CardLocation location) const;
   void SetCard(CardLocation location, CompactCard card);
   void RemoveCard(CardLocation location);

   inline Column &GetColumn(uint8_t index) { return columns[index];}
   Rank GetAcePileRank(Suit suit) const { return acePiles[(uint8_t)suit]; }
   inline CompactCard GetTower(uint8_t index) const { return towers[index]; }

   CardLocation GetCardToMoveToAce() const;
   CardLocation GetBottomColumnCardLocation(uint8_t column) const;
   CardLocation GetCardLocation(CompactCard card);
   CompactCard GetTowerCard(uint8_t tower);

   bool IsBottomOfColumn(CardLocation location) const;

public:
  static Game instance;

private:
   bool CanMoveToAce(CompactCard card) const;
   void Shuffle8(uint8_t instruction);

private:
  CompactCard deck[52];
  Rank acePiles[4];
  CompactCard towers[4];
  Column columns[10];
};

#endif  // SEAHAVEN2E_GAME_H_
