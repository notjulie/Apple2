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
/// Handy little hack that makes passing addresses to inline ASM easier
/// </summary>
union AsmByte {
   uint8_t value;
   uint8_t location[1];
};

/// <summary>
/// Our static data
/// </summary>
static uint8_t startX, startY;
static AsmByte targetX;
static AsmByte targetY;
static AsmByte suit;
static AsmByte rank;


__attribute__((noinline)) static void ChooseRandomTarget();
__attribute__((noinline)) static uint8_t Difference(uint8_t a, uint8_t b);
static void StartNextAnimation();


/// <summary>
/// Starts screensaver
/// </summary>
void ScreensaveStart()
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
   startX = 0;
   startY = a2::VBLCounter::GetCounter().lo >> 1;
   StartNextAnimation();
}

/// <summary>
/// Performs periodic actions during screensave mode
/// </summary>
void ScreensaveService()
{
   // if the last animation completed start a new one
   if (!CardAnimator::instance.Service())
   {
      startX = targetX.value;
      startY = targetY.value;
      StartNextAnimation();
   }
}

/// <summary>
/// Sets our target position to a random spot based on our various rules
/// </summary>
__attribute__((noinline)) static void ChooseRandomTarget()
{
   // declare our locals as static arrays... this way we
   // can pass them to our asm routine as addresses
   static uint8_t seed[1];

   // grab the VBLCounter as a seed for our silliness
   seed[0] = a2::VBLCounter::GetCounter().lo;

   // Go into a loop that tries a series of random target points
   // until we find one we like.  We have rules, particularly that
   // we avoid animation paths that are too vertical because our
   // vertical animations are chunky st low speed.  If we don't
   // like the point that we chose, we increment our random number
   // by a prime number and try again.
   for (; ;)
   {
      asm volatile (
      "0:\n"
         "LDX\t#0\n"

         // set initial values
         "LDA\t#%[XMax]\n"
         "STA\t%[x]\n"
         "LDA\t#%[YMax]\n"
         "STA\t%[y]\n"

         // increase our seed by a prime number each time
         "LDA\t%[seed]\n"
         "CLC\n"
         "ADC\t#37\n"
         "STA\t%[seed]\n"
         "TAY\n"

         // an even seed means we set X, odd means we set Y
         "LSR\n"
         "BCS\t1f\n"

         // even seed, set X; this ends up using the 0x04 bit
         // to decide whether to clear Y (target == top edge)
         "LSR\n"
         "LSR\n"
         "BCS\t3f\n"
         "STX\t%[y]\n"
      "3:\n"
         "CMP\t#%[XMax]+1\n"
         "BCS\t0b\n"    // x > XMax, retry
         "STA\t%[x]\n"
         "BCC\t2f\n"

      "1:\n"
         // odd seed, set Y; use the 0x04 bit to decide if
         // we set left edge or right edge
         "TYA\n"
         "CMP\t#%[YMax]+1\n"
         "BCS\t0b\n"  // y > YMax, retry
         "STA\t%[y]\n"
         "AND\t#4\n"
         "BEQ\t2f\n"
         "STX\t%[x]\n"

      "2:\n"
         // set the suit and rank
         "TYA\n"
         "AND\t#3\n"
         "STA\t%[suit]\n"

         "TYA\n"
         "LSR\n"
         "LSR\n"
         "AND\t#15\n"
         "BEQ\t0b\n"    // rank==0 invalid, retry
         "CMP\t#14\n"
         "BCS\t0b\n"    // rank > 13, invalid, retry
         "STA\t%[rank]\n"


      :  //outputs
      : [x]"i"(targetX.location), [y]"i"(targetY.location), [suit]"i"(suit.location), [rank]"i"(rank.location), [seed]"i"(seed), [XMax]"i"(XMax), [YMax]"i"(YMax) //inputs
      : "a","x","y" //clobbers
      );

      // check our results
      if (targetX.value == startY)
         continue;

      // For an angle off the vertical of 30 degrees, our x change
      // needs to be at least half of our y change.  However, the
      // x change is measured in groups of 7 pixels.  In any case,
      // testing proves that this ratio pleases me.
      if (Difference(targetX.value, startX) < (Difference(targetY.value, startY)>>3))
         continue;

      // all our tests passed, we can accept the results
      break;
   }
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
static void StartNextAnimation()
{
   // set the end position
   ChooseRandomTarget();

   // calculate a duration based on the distance
   uint8_t dx, dy;
   dx = Difference(startX, targetX.value);
   dy = Difference(startY, targetY.value);
   uint8_t distance = CardAnimator::CalculatePixelDistance(dx, dy);

   // start animating... note that if the duration is too large
   // (i.e. speed is too low) the animation gets really choppy
   // because horizontally we only stop on every 7th pixel
   CardAnimator::instance.StartFreeAnimation(
         Card((SuitOrdinal)suit.value, (Rank)rank.value),
         startX, startY,
         targetX.value, targetY.value,
         distance >> 2
         );
}

