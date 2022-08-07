
#include "Game.h"

Game Game::instance;

void Game::Initialize()
{
   uint8_t cardNumber = 0;
   for (uint8_t column=0; column<10; ++column)
   {
      for (uint8_t row=0; row<5; ++row)
      {
         columns[column].GetCard(row).SetFromCardNumber(cardNumber++);
      }
   }

   towers[0].SetNull();
   towers[1].SetFromCardNumber(cardNumber++);
   towers[2].SetFromCardNumber(cardNumber++);
   towers[3].SetNull();
}

