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

  inline Column &GetColumn(uint8_t index) { return columns[index];}
  inline CompactCard GetAcePileCard(Suit suit) const {
                return acePiles[(uint8_t)suit];
              }
  inline CompactCard GetTower(uint8_t index) const { return towers[index]; }

  CompactCard GetCard(CardLocation location) const;
  CardLocation GetCardToMoveToAce() const;
  void SetCard(CardLocation location, CompactCard card);
  CardLocation GetBottomColumnCardLocation(uint8_t column);
  CardLocation GetCardLocation(CompactCard card);
  CompactCard GetTowerCard(uint8_t tower);

  bool IsBottomOfColumn(CardLocation location) const;

 public:
  static Game instance;

 private:
  void Shuffle8(uint8_t instruction);

 private:
  uint8_t deck[52];
  CompactCard acePiles[4];
  CompactCard towers[4];
  Column columns[10];
};

#endif  // SEAHAVEN2E_GAME_H_
