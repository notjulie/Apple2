
#include "Game.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "Random.h"

Game Game::instance;

/** \brief
 * Shuffles and deals out a new game
 */
void Game::Shuffle16(uint16_t instruction)
{
   // create unshuffled deck
   for (uint8_t i=0; i<52; ++i)
      deck[i] = i;

   // shuffle 8 times according to high byte
   Shuffle8(instruction >> 8);

   // shuffle 8 times according to low byte
   Shuffle8((uint8_t)instruction);

   // shuffle
   /*for (uint8_t i=0; i<5; ++i)
   {
      for (int8_t cardIndex=51; cardIndex>=0; --cardIndex)
      {
         uint8_t randomIndex = 63 & Random::GetByte();
         if (randomIndex > 52)
            randomIndex -= 52;

         uint8_t temp = deck[cardIndex];
         deck[cardIndex] = deck[randomIndex];
         deck[randomIndex] = temp;
      }
   }*/

   // deal
   uint8_t cardIndex = 0;
   for (uint8_t column=0; column<10; ++column)
   {
      for (uint8_t row=0; row<5; ++row)
      {
         columns[column].GetCard(row).SetFromCardNumber(deck[cardIndex++]);
      }
   }

   towers[0].SetNull();
   towers[1].SetFromCardNumber(deck[cardIndex++]);
   towers[2].SetFromCardNumber(deck[cardIndex++]);
   towers[3].SetNull();
}

void Game::Shuffle8(uint8_t instruction)
{
   uint8_t deckCopy[52];

   // we have two types of shuffling, and we choose one each time through
   // based on the bits in the instruction
   for (int i=0; i<8; ++i)
   {
      c6502::memcpy8(deckCopy, deck, 52);

      if (instruction & 1)
      {
         uint8_t index = 0;
         for (int j=0; j<26; ++j)
         {
            deck[index++] = deckCopy[j];
            deck[index++] = deckCopy[j + 26];
         }
      }
      else
      {
         uint8_t index = 0;
         for (int j=0; j<52; ++j)
         {
            deck[j] = deckCopy[index];
            index += 7;
            if (index >= 52)
               index -= 52;
         }
      }

      instruction >>= 1;
   }
}


void Card::SetFromCardNumber(uint8_t cardNumber)
{
   suit = (Suit)(cardNumber & 3);
   rank = (Rank)(1 + (cardNumber >> 2));
}
