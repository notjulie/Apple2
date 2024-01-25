// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Screensave.h"

#include <Apple2Lib/HGR.h>
#include <Apple2Lib/VBLCounter.h>
#include "CardAnimator.h"
#include "SHAssert.h"


/// <summary>
/// constants
/// </summary>
static constexpr uint8_t XMax = a2::HGRByteWidth - CardLocations::CardByteWidth;
static constexpr uint8_t YMax = a2::HGRHeight - (CardHeight + CardLocations::CardShadowHeight);

/// <summary>
/// Our global instance
/// </summary>
Screensave Screensave::instance;

__attribute__((noinline)) static uint8_t Difference(uint8_t a, uint8_t b);


/// <summary>
/// Starts screensaver
/// </summary>
void Screensave::Start()
{
   // erase the screen
   CardAnimator::instance.Erase();

   // So, I don't think I care where the screensaver starts on the screen,
   // but rather than trying for a perfectly random location I'm just going
   // to pick a random spot on the left edge of the screen.  The left edge
   // just has a "I've been reading from left-to-right my whole life" feel
   // to it.

   // I'm assuming that 0-127 is a valid range for the Y coordinate
   static_assert(YMax >= 127);
   StartNextAnimation(0, a2::VBLCounter::GetCounter().lo >> 1);
}

/// <summary>
/// Performs periodic actions during screensave mode
/// </summary>
void Screensave::Service()
{
   // let CardAnimator do its work
   CardAnimator::instance.Service();

   // if the last animation completed start a new one
   if (!CardAnimator::instance.IsAnimating())
   {
      StartNextAnimation(targetX, targetY);
   }
}

/// <summary>
/// Sets our target position to a random spot based on our various rules
/// </summary>
void Screensave::ChooseRandomTarget(uint8_t startX, uint8_t startY)
{
   // grab the VBLCounter as a seed for our silliness
   uint8_t seed = a2::VBLCounter::GetCounter().lo;

   // Go into a loop that tries a series of random target points
   // until we find one we like.  We have rules, particularly that
   // we avoid animation paths that are too vertical because our
   // vertical animations are chunky st low speed.  If we don't
   // like the point that we chose, we increment our random number
   // by a prime number and try again.
   uint8_t x, y, suit, rank;
   for (; ; seed += 37)
   {
      x = XMax;
      y = YMax;

      asm volatile (
         "LDX\t#0\n"

         // an even seed means we set X, odd means we set Y
         "LDA\t%4\n"
         "LSR\n"
         "BCS\t1f\n"

         // even seed, set X; this ends up using the 0x04 bit
         // to decide whether to clear Y (target == top edge)
         "LSR\n"
         "LSR\n"
         "STA\t%0\n"
         "BCS\t2f\n"
         "STX\t%1\n"
         "BCC\t2f\n"

      "1:\n"
         // odd seed, set Y; use the 0x04 bit to decide if
         // we set left edge or right edge
         "LDA\t%4\n"
         "STA\t%1\n"
         "AND\t#4\n"
         "BEQ\t2f\n"
         "STX\t%0\n"

      "2:\n"
         // set the suit and rank
         "LDA\t%4\n"
         "AND\t#3\n"
         "STA\t%2\n"

         "LDA\t%4\n"
         "LSR\n"
         "LSR\n"
         "AND\t#15\n"
         "STA\t%3\n"


      : "+r"(x), "+r"(y), "+r"(suit), "+r"(rank) //outputs
      : "r"(seed) //inputs
      : "a","x" //clobbers
      );

      // check our results
      if (rank==0 || rank>13)
         continue;
      if (x > XMax)
         continue;
      if (y > YMax || y == startY)
         continue;

      // For an angle off the vertical of 30 degrees, our x change
      // needs to be at least half of our y change.  However, the
      // x change is measured in groups of 7 pixels.  In any case,
      // testing proves that this ratio pleases me.
      if (Difference(x, startX) < (Difference(y, startY)>>3))
         continue;

      // all our tests passed, we can accept the results
      break;
   }

   targetX = x;
   targetY = y;
   cardInMotion = Card(Suit::FromOrdinal(suit), (Rank)rank);
}


/// <summary>
/// Calculates the absolute value difference between two bytes
/// </summary>
__attribute__((noinline)) static uint8_t Difference(uint8_t a, uint8_t b)
{
   uint8_t result;

   asm volatile (
      // subtract a from b
      "PHA\n"
      "SEC\n"
      "SBC\t%1\n"
      "BCS\t1f\n"

      // we had to borrow, negate the result
      "EOR\t#$FF\n"
      "ADC\t#$01\n"

   "1:\n"
      "TAX\n"
      "PLA\n"

   : "=x"(result) // outputs
   : "r"(a), "a"(b) // input
   :  // clobbers
   );

   return result;
}


/// <summary>
/// Starts an animation from the given location to a randomly chosen
/// destination.
/// </summary>
void Screensave::StartNextAnimation(uint8_t startX, uint8_t startY)
{
   // set the end position
   ChooseRandomTarget(startX, startY);

   // calculate a duration based on the distance
   uint8_t dx, dy;
   dx = Difference(startX, targetX);
   dy = Difference(startY, targetY);
   uint8_t distance = CardAnimator::CalculatePixelDistance(dx, dy);

   // start animating
   CardAnimator::instance.StartFreeAnimation(
         cardInMotion,
         startX, startY,
         targetX, targetY,
         distance >> 1
         );
}

