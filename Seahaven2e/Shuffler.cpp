// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Shuffler.h"
#include <C6502/Memory.h>


/// <summary>
/// Shuffles a deck based on the integer seed value
/// </summary>
__attribute__((noinline)) void Shuffler::Shuffle16(c6502::Int16 instruction)
{
   // create unshuffled deck
   for (uint8_t i=0; i < 52; ++i)
      deck[i] = Card::FromOrdinal(i);

   // shuffle 8 times according to high byte
   Shuffle8(instruction.hi);

   // shuffle 8 times according to low byte
   Shuffle8(instruction.lo);
}


/// <summary>
/// Shuffles our deck based on the eight bit instruction
/// </summary>
void Shuffler::Shuffle8(uint8_t instruction) {
   Card deckCopy[52];

   Card *pDeck = &deck[0];
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

      "loop:\n"
         // deckCopy[index++] = deck[25 - j];
         "TXA\n"
         "TAY\n"
         "LDA\t(%2),y\n"
         "LDY\t%0\n"
         "STA\t(%3),y\n"
         "INC\t%0\n"

         // deckCopy[index++] = deck[j + 26];
         "LDY\t%1\n"
         "LDA\t(%2),y\n"
         "LDY\t%0\n"
         "STA\t(%3),y\n"
         "INC\t%0\n"
         "INC\t%1\n"

         // loop count
         "DEX\n"
         "BPL\tloop\n"
      : "=r"(index),"=r"(index2) //outputs
      : "r"(pDeck),"r"(pDeckCopy),"r"(index),"r"(index2) //inputs
      : "a","x","y" //clobbers
      );

      // was formerly this:
      /* index = 0;
      for (int j=0; j < 26; ++j)
      {
         deckCopy[index++] = deck[25 - j];
         deckCopy[index++] = deck[j + 26];
      }*/

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

      instruction >>= 1;
   }
}


