
#ifndef GAME_H
#define GAME_H

#include "Rank.h"
#include "Suit.h"

class Card
{
public:
   inline void SetFromCardNumber(uint8_t cardNumber) {
      rank = (Rank)(cardNumber & 3);
      suit = (Suit)(1 + (cardNumber >> 2));
   }

private:
   Suit suit;
   Rank rank = Rank::Null;
};

class Column
{
public:
   inline Card &GetCard(uint8_t index) { return cards[index];}

private:
   Card  cards[5];
};

class Game
{
public:
   Game();

   inline Column &GetColumn(uint8_t index) { return columns[index];}

private:
   Card towers[4];
   Column columns[10];
};

#endif // GAME_H
