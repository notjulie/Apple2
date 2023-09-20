
#include "Deck.h"

#include <C6502/Memory.h>


/// <summary>
/// A simple assembly lookup so that one op code can turn a tower
/// index into an index into our cards array
/// </summary>
static uint8_t GetTowerCardIndex(uint8_t towerIndex)
{
   static constexpr uint8_t towerCardIndices[5] = {52, 50, 51, 53};

   uint8_t result;

   asm volatile (
      "LDY\t%[towerCardIndices],x\n"
   : "=y"(result) //outputs
   : "x"(towerIndex), [towerCardIndices] "i"(towerCardIndices) //inputs
   : //clobbers
   );

   return result;
}


static uint8_t GetColumnCardIndex(uint8_t column, uint8_t row)
{
   static constexpr uint8_t rowOffset[5] = {0, 10, 20, 30, 40};

   uint8_t result;

   asm volatile (
      "TYA\n"
      "CLC\n"
      "ADC\t%[rowOffset],x\n"
   : "=a"(result) //outputs
   : "x"(row), "y"(column), [rowOffset] "i"(rowOffset) //inputs
   : //clobbers
   );

   return result;
}

Card Deck::GetColumnCard(uint8_t column, uint8_t row) const
{
   return cards[GetColumnCardIndex(column, row)];
}


Card Deck::GetTower(uint8_t index) const
{
   return cards[GetTowerCardIndex(index)];
}


void Deck::SetColumnCard(uint8_t column, uint8_t row, Card card)
{
   cards[GetColumnCardIndex(column, row)] = card;
}


void Deck::SetTower(uint8_t index, Card card)
{
   cards[GetTowerCardIndex(index)] = card;
}


__attribute__((noinline)) void Deck::Shuffle(c6502::Int16 instruction)
{
   // create unshuffled deck
   for (uint8_t i=0; i < 52; ++i)
      cards[i] = Card::FromOrdinal(i);

   // shuffle 8 times according to high byte
   Shuffle8(instruction.hi);

   // shuffle 8 times according to low byte
   Shuffle8(instruction.lo);

   // clear the two towers that start empty
   cards[52] = cards[53] = Card::Null();
}


/// <summary>
/// Shuffles our deck based on the eight bit instruction
/// </summary>
__attribute__((noinline)) void Deck::Shuffle8(uint8_t instruction) {
   Card deckCopy[52];

   Card *pDeck = &cards[0];
   Card *pDeckCopy = &deckCopy[0];

   // we have two types of shuffling, and we choose one each time through
   // based on the bits in the instruction
   for (int i=0; i < 8; ++i)
   {
      // shuffle from deck to deckCopy
      uint8_t index;
      uint8_t index2;
      asm volatile (
         // clear index
         "LDX\t#0\n"
         "STX\t%0\n"

         // index2 is our (j + 26) counter
         "LDX\t#26\n"
         "STX\t%1\n"

         // x is our (25 - j) counter
         "DEX\n"

      "1:\n"
         // deckCopy[index++] = deck[25 - j];
         "TXA\n"
         "TAY\n"
         "LDA\t(%3),y\n"
         "LDY\t%0\n"
         "STA\t(%4),y\n"
         "INC\t%0\n"

         // deckCopy[index++] = deck[j + 26];
         "LDY\t%1\n"
         "LDA\t(%3),y\n"
         "LDY\t%0\n"
         "STA\t(%4),y\n"
         "INC\t%0\n"
         "INC\t%1\n"

         // loop count
         "DEX\n"
         "BPL\t1b\n"

         // load up our index and increment (index2) base on the low
         // bit of the instruction
         "LDA\t#17\n"
         "LDX\t#19\n"
         "LSR\t%2\n"
         "BCS\t1f\n"
         "LDA\t#23\n"
         "LDX\t#7\n"
      "1:\n"
         "STX\t%1\n"

         // index is our loop counter
         "LDX\t#51\n"
         "STX\t%0\n"

      "3:\n"
         "TAY\n"
         "PHA\n"
         "LDA\t(%4),y\n"
         "LDY\t%0\n"
         "STA\t(%3),y\n"
         "PLA\n"

         "CLC\n"
         "ADC\t%1\n"
         "CMP\t#52\n"
         "BMI\t1f\n"
         "SEC\n"
         "SBC\t#52\n"
      "1:\n"
         "DEC\t%0\n"
         "BPL\t3b\n"


      : "=r"(index),"=r"(index2),"=r"(instruction) //outputs
      : "r"(pDeck),"r"(pDeckCopy),"r"(index),"r"(index2),"r"(instruction) //inputs
      : "a","x","y" //clobbers
      );

      // was formerly this:
      /* index = 0;
      for (int j=0; j < 26; ++j)
      {
         deckCopy[index++] = deck[25 - j];
         deckCopy[index++] = deck[j + 26];
      }

      // shuffle from deckCopy to deck
      uint8_t increment;
      if (instruction & 1)
      {
         index = 17;
         increment = 19;
      }
      else
      {
         index = 23;
         increment = 7;
      }
      for (int j=51; j >= 0; --j)
      {
         deck[j] = deckCopy[index];
         index += increment;
         if (index >= 52)
            index -= 52;
      }

      instruction >>= 1;*/
   }
}



