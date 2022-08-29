
#ifndef GAME_H
#define GAME_H

#include "CardLocation.h"
#include "Rank.h"
#include "Suit.h"

/** \brief
 * Represents a card in our game table
 */
class Card
{
public:
   void SetFromCardNumber(uint8_t cardNumber);

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
   void Shuffle16(uint16_t instruction);

   inline Column &GetColumn(uint8_t index) { return columns[index];}
   inline Card &GetTower(uint8_t index) { return towers[index]; }

   CardLocation GetAceToMove();

public:
   static Game instance;

private:
   void Shuffle8(uint8_t instruction);

private:
   uint8_t deck[52];
   Card towers[4];
   Column columns[10];
};

#endif // GAME_H
