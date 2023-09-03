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
   uint8_t index;

   // we have two types of shuffling, and we choose one each time through
   // based on the bits in the instruction
   for (int i=0; i < 8; ++i)
   {
      c6502::memcpy8(deckCopy, deck, 52);

      index = 0;
      for (int j=0; j < 26; ++j)
      {
         deck[index++] = deckCopy[25 - j];
         deck[index++] = deckCopy[j + 26];
      }

      uint8_t increment;
      c6502::memcpy8(deckCopy, deck, 52);
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


