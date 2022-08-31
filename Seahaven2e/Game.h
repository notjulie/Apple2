
#ifndef GAME_H
#define GAME_H

#include "Card.h"
#include "CardLocation.h"


/** \brief
 * Represents a column on the game board
 */
class Column
{
public:
   inline Card GetCard(uint8_t index) const { return cards[index]; }
   void SetCard(uint8_t index, Card card) { cards[index] = card; }

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
   inline Card GetAcePileCard(Suit suit) const { return acePiles[(uint8_t)suit]; }
   inline Card &GetTower(uint8_t index) { return towers[index]; }

   Card GetCard(CardLocation location) const;
   CardLocation GetCardToMoveToAce() const;
   void SetCard(CardLocation location, Card card);

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

#endif // GAME_H
