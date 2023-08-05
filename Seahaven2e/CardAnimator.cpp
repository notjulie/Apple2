// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "CardAnimator.h"

#include <Apple2Lib/VBLCounter.h>
#include <Apple2Lib/ROM.h>
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"
#include "SHAssert.h"


/// <summary>
/// Our global instance
/// </summary>
CardAnimator CardAnimator::instance;


/// <summary>
/// Constructor... leaves the object uninitialized; Initialize
/// must be called
/// </summary>
CardAnimator::CardAnimator()
{
}


/// <summary>
/// Performs one-time initialization
/// </summary>
void CardAnimator::Initialize()
{
   state = State::Idle;
   onscreenPage = AnimationPage::Page1();
   offscreenPage = AnimationPage::Page2();
}



/// <summary>
/// Draws the game as it currently sits; we are responsible for
/// this since we maintain the states of the HGR pages
/// </summary>
__attribute__((noinline)) void CardAnimator::DrawGame()
{
   // draw to offscreen page
   offscreenPage.DrawGame();

   // show
   SwapPages();

   // copy to the offscreen buffer
   offscreenPage.CopyFrom(onscreenPage);

   // both pages are the same and up to date
   state = State::Idle;

   // and the cursor is no more
   Cursor::instance.CursorHasBeenObliterated();
}


/// <summary>
///   Starts an animation of a card from one position to another
/// </summary>
void CardAnimator::StartAnimation(
      CompactCard card,
      CardLocation end)
{
   // save parameters
   endLocation = end;

   // step 0: hide the cursor
   Cursor::instance.Hide();

   // step 1: remove the card from its current position
   CardLocation start = Game::instance.GetCardLocation(card);
   Game::instance.RemoveCard(start);

   // set the bounds of the animation
   currentPosition[(uint8_t)Coordinate::X] = start.GetX();
   currentPosition[(uint8_t)Coordinate::Y] = start.GetY() - CardLocations::CardShadowHeight;
   targetPosition[(uint8_t)Coordinate::X] = end.GetX();
   targetPosition[(uint8_t)Coordinate::Y] = end.GetY() - CardLocations::CardShadowHeight;
   StartPositionTracker((uint8_t)Coordinate::X);
   StartPositionTracker((uint8_t)Coordinate::Y);

   // calculate the duration
   uint8_t pixelDistance = CalculatePixelDistance(distance[(uint8_t)Coordinate::X], distance[(uint8_t)Coordinate::Y]);
   duration = pixelDistance >> 2;

   timeLeft = duration;
   lastVBLCount = a2::VBLCounter::GetCounter();
   cardToMove = card;

   // draw the game without the card
   offscreenPage.EraseCard(start);

   // draw the card at its original position, saving the background
   offscreenPage.MoveCard(
         cardToMove,
         currentPosition[(uint8_t)Coordinate::X],
         currentPosition[(uint8_t)Coordinate::Y]
         );

   // switch
   SwapPages();

   // draw the game without the card
   offscreenPage.EraseCard(start);

   // set the state
   state = State::Animating;
}



__attribute__((noinline)) void CardAnimator::StartPositionTracker(uint8_t i)
{
   if (targetPosition[i] > currentPosition[i])
   {
      distance[i] = targetPosition[i] - currentPosition[i];
      direction[i] = 1;
   }
   else
   {
      distance[i] = currentPosition[i] - targetPosition[i];
      direction[i] = -1;
   }
   numerator[i] = 0;
}


/// <summary>
/// Calculates an approximate hypotenuse
/// </summary>
uint8_t CardAnimator::CalculatePixelDistance(uint8_t dx, uint8_t dy)
{
   // So for starters each x location is 7 pixels, y = 1 pixels, so
   // I want 7 * dx for the x distance.  But also I need to divide both
   // by two so we can be sure to fit the result into a byte

   // and the compiler tries to get clever and isn't really on operations
   // like this, so we go manual on some of this
   uint8_t xPixels;
   asm volatile (
      "TXA\n"
      "LSR\n"
      "STA\t%0\n"

      "TXA\n"
      "ASL\n"
      "ASL\n"
      "SEC\n"
      "SBC\t%0\n"
      "STA\t%0\n"
   : "=r"(xPixels) // outputs
   : "x"(dx) // input
   : "a"// clobbers
   );

   dy >>= 1;

   // I approximate the distance as the larger leg +
   // half of the smaller leg
   if (dy > xPixels)
      return dy + (xPixels>>1);
   else
      return xPixels + (dy>>1);
}


/// \brief
///   Updates the state of the animation
///
__attribute__((noinline)) void CardAnimator::Service()
{
   switch (state) {
   case State::Idle:
      break;

   case State::Animating:
      // update the position, move the card
      UpdatePosition();
      offscreenPage.MoveCard(
            cardToMove,
            currentPosition[(uint8_t)Coordinate::X],
            currentPosition[(uint8_t)Coordinate::Y]
            );
      SwapPages();

      // if we're done...
      if (timeLeft == 0)
      {
         // end the animation on the onscreen page
         onscreenPage.EndAnimation();

         // finish the animation on the offscreen page
         offscreenPage.MoveCard(
               cardToMove,
               currentPosition[(uint8_t)Coordinate::X],
               currentPosition[(uint8_t)Coordinate::Y]
               );
         offscreenPage.EndAnimation();

         // update the game object and our state
         Game::instance.SetCard(endLocation, cardToMove);
         state = State::Idle;
         break;
      }
      break;

   case State::MovingColumnToColumn:
      ServiceColumnToColumnMove();
      break;
   }
}


__attribute__((noinline)) void CardAnimator::ServiceColumnToColumnMove()
{
   // update the position, move the card
   UpdatePosition();
   offscreenPage.MoveCard(
         cardToMove,
         currentPosition[(uint8_t)Coordinate::X],
         currentPosition[(uint8_t)Coordinate::Y]
         );
   SwapPages();

   // if we're done...
   if (timeLeft == 0)
   {
      // so the trick is that for every card except the first one
      // we only draw the card top when it gets to its final location
      if (cardBeingMoved == numberOfCardsToMove - 1)
      {
         // end the animation on the onscreen page
         onscreenPage.EndAnimation();

         // finish the animation on the offscreen page
         offscreenPage.MoveCard(
               cardToMove,
               currentPosition[(uint8_t)Coordinate::X],
               currentPosition[(uint8_t)Coordinate::Y]
               );
         offscreenPage.EndAnimation();
      }
      else
      {
         offscreenPage.MoveCardTop(
               cardToMove,
               currentPosition[(uint8_t)Coordinate::X],
               currentPosition[(uint8_t)Coordinate::Y]
               );
         offscreenPage.EndAnimation();
         SwapPages();

         offscreenPage.MoveCardTop(
               cardToMove,
               currentPosition[(uint8_t)Coordinate::X],
               currentPosition[(uint8_t)Coordinate::Y]
               );
         offscreenPage.EndAnimation();
      }

      // next move
      NextColumnToColumnMove();
   }
}


void CardAnimator::SwapPages()
{
   AnimationPage temp = onscreenPage;
   onscreenPage = offscreenPage;
   offscreenPage = temp;

   onscreenPage.Show();
}


/// <summary>
/// Attempts to move a group of one to five cards from one column to
/// another.  The locations have been verified, but the rest of the details
/// of the move have not.
/// </summary>
__attribute__((noinline)) void CardAnimator::StartMoveColumnToColumn(CardLocation from, CardLocation to)
{
   // figure out if we have a valid group to move and how big it is
   numberOfCardsToMove = Game::instance.GetSizeOfMoveToColumnGroup(from);
   if (numberOfCardsToMove == 0)
      return;

   // make sure we have the tower space to allow it
   if (numberOfCardsToMove > Game::instance.GetNumberOfAvailableTowers() + 1)
      return;

   // make a list of the cards to move; we can afford the 15 bytes
   for (int i=0; i<numberOfCardsToMove; ++i)
   {
      cardsToMove[i] = Game::instance.GetCard(from);
      startLocations[i] = from;
      endLocations[i] = to;

      from = from.Down();
      to = to.Down();
   }

   // we move the cards from bottom up
   cardBeingMoved = numberOfCardsToMove;
   NextColumnToColumnMove();
}


/// <summary>
/// Checks to see if there are more cards to move and either starts
/// moving the next one or wraps things up and goes back to State::Idle
/// </summary>
void CardAnimator::NextColumnToColumnMove()
{
   // if we are out of cards to move we need to finally update
   // the game
   if (cardBeingMoved == 0)
   {
      // actually move them... remember that columns like to have cards
      // added and removed lick a stack
      for (int i=0; i<numberOfCardsToMove; ++i)
         Game::instance.SetCard(endLocations[i], cardsToMove[i]);

      // done
      state = State::Idle;
      return;
   }

   // next
   --cardBeingMoved;
   StartAnimation(cardsToMove[cardBeingMoved], endLocations[cardBeingMoved]);
   state = State::MovingColumnToColumn;
}


/// \brief
///   Calculates the new position
///
void CardAnimator::UpdatePosition() {
   if (timeLeft == 0)
      return;

   // snapshot the time
   uint8_t now = a2::VBLCounter::GetCounter();

   // step once for each tick of the clock
   while (lastVBLCount != now)
   {
      UpdatePositionTracker((uint8_t)Coordinate::X);
      UpdatePositionTracker((uint8_t)Coordinate::Y);

      ++lastVBLCount;
      if (--timeLeft == 0)
         break;
   }
}


__attribute__((noinline)) void CardAnimator::UpdatePositionTracker(uint8_t i)
{
   numerator[i] += distance[i];
   while (numerator[i] >= duration)
   {
      currentPosition[i] += direction[i];
      numerator[i] -= duration;
   }
}
