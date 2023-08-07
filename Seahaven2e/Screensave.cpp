// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Screensave.h"

#include "CardAnimator.h"
#include "SHAssert.h"


Screensave Screensave::instance;


void Screensave::Start()
{
   CardAnimator::instance.Erase();

   CardAnimator::instance.StartFreeAnimation(
         Card(Suit::Clubs(), Rank::Three),
         36, 100, 0, 100,
         100
         );
}

void Screensave::Service()
{
   CardAnimator::instance.Service();
   if (!CardAnimator::instance.IsAnimating())
      assert(0);
}
