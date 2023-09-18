
#ifndef DECK_H
#define DECK_H


#include <C6502/Int6502.h>
#include "Card.h"


/// <summary>
/// This is a collection of 54 cards... this is basically the 52 cards
/// that are dealt and the two empty towers
/// </summary>
class Deck {
public:
   Deck() {}

   Card GetColumnCard(uint8_t column, uint8_t row) const;
   Card GetTower(uint8_t index) const;
   void SetColumnCard(uint8_t column, uint8_t row, Card card);
   void SetTower(uint8_t index, Card card);
   void Shuffle(c6502::Int16 instruction);

private:
   // for convenience we include two extra cards to represent the two
   // towers that start out empty...
   Card cards[54];
};


#endif // DECK_H


