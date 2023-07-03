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
  inline Card GetAcePileCard(Suit suit) const {
                return acePiles[(uint8_t)suit];
              }
  inline Card &GetTower(uint8_t index) { return towers[index]; }

  Card GetCard(CardLocation location) const;
  CardLocation GetCardToMoveToAce() const;
  void SetCard(CardLocation location, Card card);
  CardLocation GetBottomColumnCardLocation(uint8_t column);

 public:
  static Game instance;

 private:
  void Shuffle8(uint8_t instruction);

 private:
  uint8_t deck[52];
  Card acePiles[4];
  Card towers[4];
  Column columns[10];
};

#endif  // SEAHAVEN2E_GAME_H_
