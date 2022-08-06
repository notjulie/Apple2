
#ifndef GAME_H
#define GAME_H

#include "Rank.h"
#include "Suit.h"

class Card
{
public:
   inline void SetFromCardNumber(uint8_t cardNumber) {
      suit = (Suit)(cardNumber & 3);
      rank = (Rank)(1 + (cardNumber >> 2));
   }

   inline Rank GetRank() const { return rank; }
   inline Suit GetSuit() const { return suit; }

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
   void Initialize();
   inline Column &GetColumn(uint8_t index) { return columns[index];}

public:
   static Game instance;

private:
   Card towers[4];
   Column columns[10];
};

#endif // GAME_H
