
#ifndef GAME_H
#define GAME_H

#include "Rank.h"
#include "Suit.h"

/** \brief
 * Represents a card in our game table
 */
class Card
{
public:
   inline void SetFromCardNumber(uint8_t cardNumber) {
      suit = (Suit)(cardNumber & 3);
      rank = (Rank)(1 + (cardNumber >> 2));
   }

   inline Rank GetRank() const { return rank; }
   inline Suit GetSuit() const { return suit; }
   inline bool IsNull() const { return rank == Rank::Null; }
   inline void SetNull() { rank = Rank::Null; }

private:
   Suit suit;
   Rank rank;
};


/** \brief
 * Represents a column on the game board
 */
class Column
{
public:
   inline Card &GetCard(uint8_t index) { return cards[index];}

private:
   Card  cards[5];
};


/** \brief
 * Represents the game as a whole
 */
class Game
{
public:
   void Initialize();
   inline Column &GetColumn(uint8_t index) { return columns[index];}
   inline Card &GetTower(uint8_t index) { return towers[index]; }

public:
   static Game instance;

private:
   Card towers[4];
   Column columns[10];
};

#endif // GAME_H
