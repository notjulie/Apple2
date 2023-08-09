// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Screensave.h"

#include <Apple2Lib/HGR.h>
#include <Apple2Lib/VBLCounter.h>
#include "CardAnimator.h"
#include "SHAssert.h"


Screensave Screensave::instance;

static constexpr uint8_t XMax = a2::HGRByteWidth - CardLocations::CardByteWidth;
static constexpr uint8_t YMax = a2::HGRHeight - (CardHeight + CardLocations::CardShadowHeight);

/// <summary>
/// Starts screensaver
/// </summary>
void Screensave::Start()
{
   // erase the screen
   CardAnimator::instance.Erase();

   // choose a random-ish edge and pretend that that's where out last
   // animation ended
   uint8_t now = a2::VBLCounter::GetCounter();
   targetEdge = (Edge)(now & 3);

   // choose a random target on that edge
   ChooseRandomTarget();

   // now we at like we just finished animating to that target and
   // animate to the next
   StartNextAnimation();
}

void Screensave::Service()
{
   CardAnimator::instance.Service();
   if (!CardAnimator::instance.IsAnimating())
      StartNextAnimation();
}

/// <summary>
/// Sets our target position to a random spot on the given edge
/// </summary>
void Screensave::ChooseRandomTarget()
{
   // we want a random number from 1 to 3 as an offset to the
   // last edge
   uint8_t newEdge = (uint8_t)targetEdge + 1;
   uint8_t now = a2::VBLCounter::GetCounter();
   if (now > 171)
      ++newEdge;
   if (now > 85)
      ++newEdge;

   // set the target edge
   targetEdge = (Edge)(newEdge & 3);

   // pick a random position on that edge
   switch (targetEdge)
   {
   case Edge::Left:
      targetX = 0;
      targetY = GetRandomY();
      break;

   case Edge::Right:
      targetX = XMax;
      targetY = GetRandomY();
      break;

   case Edge::Top:
      targetX = GetRandomX();
      targetY = 0;
      break;

   case Edge::Bottom:
      targetX = GetRandomX();
      targetY = YMax;
      break;
   }
}


uint8_t Screensave::GetRandomX()
{
   // we are designed to return a value in the range 0..36, so make sure
   // that's the expectation
   static_assert(XMax==36, "GetRandomX assumes a range of 0 to 36");

   uint8_t now = a2::VBLCounter::GetCounter();
   return
      (now >> 3) + // (0 to 31)
      ((now >> 1) & 3) + // 0 to 3
      ((now & 1) << 1); // 0 to 2
}


uint8_t Screensave::GetRandomY()
{
   // we are designed to return a value in the range 0..159, so make sure
   // that's the expectation
   static_assert(YMax==158, "GetRandomX assumes a range of 0 to 158");

   uint8_t now = a2::VBLCounter::GetCounter();
   return
      (now >> 1) + // (0 to 127)
      (now >> 3); // 0 to 31
}


__attribute__((noinline)) static uint8_t Difference(uint8_t a, uint8_t b)
{
   uint8_t result;

   asm volatile (
      // subtract a from b
      "PHA\n"
      "SEC\n"
      "SBC\t%1\n"
      "BCS\t1f\n"

      // negate
      "EOR\t#$FF\n"
      "ADC\t#$01\n"
      "LDA\t$0\n"

   "1:\n"
      "TAX\n"
      "PLA\n"

   : "=x"(result) // outputs
   : "r"(a), "a"(b) // input
   :  // clobbers
   );

   return result;
}


void Screensave::StartNextAnimation()
{
   // grab the start position
   uint8_t startX = targetX;
   uint8_t startY = targetY;

   // set the end position
   ChooseRandomTarget();

   // calculate a duration based on the distance
   uint8_t dx, dy;
   dx = Difference(startX, targetX);
   dy = Difference(startY, targetY);
   uint8_t distance = CardAnimator::CalculatePixelDistance(dx, dy);

   // start animating
   CardAnimator::instance.StartFreeAnimation(
         Card(Suit::Clubs(), Rank::Three),
         startX, startY,
         targetX, targetY,
         distance >> 2
         );
}

