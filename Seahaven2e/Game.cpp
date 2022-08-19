
#include "Game.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "Random.h"

Game Game::instance;

/** \brief
 * Shuffles and deals out a new game
 */
void Game::Shuffle()
{
   // create unshuffled deck
   static uint8_t deck[52];
   for (uint8_t i=0; i<52; ++i)
      deck[i] = i;

   // shuffle
   for (uint8_t i=0; i<5; ++i)
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
   }

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

