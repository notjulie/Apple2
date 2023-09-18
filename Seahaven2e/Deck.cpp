
#include "Deck.h"

#include <C6502/Memory.h>
#include "Shuffler.h"

/// <summary>
/// look up table for avoiding multiplying by 10
/// </summary>
static constexpr uint8_t rowOffset[5] = {0, 10, 20, 30, 40};


/// <summary>
/// look up table for converting requested tower to card index
/// </summary>
static constexpr uint8_t towerTransform[4] = { 52, 50, 51, 53 };



Card Deck::GetColumnCard(uint8_t column, uint8_t row) const
{
   return cards[rowOffset[row] + column];
}


Card Deck::GetTower(uint8_t index) const
{
   return cards[towerTransform[index]];
}


void Deck::SetColumnCard(uint8_t column, uint8_t row, Card card)
{
   cards[rowOffset[row] + column] = card;
}


void Deck::SetTower(uint8_t index, Card card)
{
   cards[towerTransform[index]] = card;
}


void Deck::Shuffle(c6502::Int16 instruction)
{
   Shuffler shuffler;
   shuffler.Shuffle16(instruction);

   c6502::memcpy8(&cards, shuffler.deck, 52);
   cards[52] = cards[53] = Card::Null();
}

