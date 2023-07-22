// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "CardAnimator.h"

#include <Apple2Lib/VBLCounter.h>
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"


/// <summary>
/// Our global instance
/// </summary>
CardAnimator CardAnimator::instance;


/// <summary>
/// Initializes a new instance of class CardAnimator
/// </summary>
CardAnimator::CardAnimator()
{
   // all items that need to be initialized are done so inline in
   // the declaration
}


/// <summary>
/// Draws the game as it currently sits; we are responsible for
/// this since we maintain the states of the HGR pages
/// </summary>
void CardAnimator::DrawGame()
{
   // draw to page 1
   page1.DrawGame();

   // show page 1
   page1.Show();

   // copy to the offscreen buffer
   page2.CopyFrom(page1);

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
   currentX = start.GetX();
   currentY = start.GetY() - CardLocations::CardShadowHeight;
   targetX = end.GetX();
   targetY = end.GetY() - CardLocations::CardShadowHeight;
   if (targetX > currentX) {
      distanceX = targetX - currentX;
      directionX = 1;
   } else {
      distanceX = currentX - targetX;
      directionX = -1;
   }
   if (targetY > currentY) {
      distanceY = targetY - currentY;
      directionY = 1;
   } else {
      distanceY = currentY - targetY;
      directionY = -1;
   }

   // calculate the duration
   uint8_t pixelDistance = CalculatePixelDistance(distanceX, distanceY);
   duration = pixelDistance >> 2;

   timeLeft = duration;
   numeratorX = numeratorY = 0;
   lastVBLCount = a2::VBLCounter::GetCounter();
   cardToMove = card;

   // draw the game without the card on page 2
   page2.EraseCard(start);

   // draw the card at its original position, saving the background
   page2.MoveCard(cardToMove, currentX, currentY);

   // switch to page 2
   page2.Show();
   showingPage1 = false;

   // draw the game without the card on page 1
   page1.EraseCard(start);

   // set the state
   state = State::Animating;
}


/// <summary>
/// Calculates an approximate hypotenuse
/// </summary>
uint8_t CardAnimator::CalculatePixelDistance(uint8_t dx, uint8_t dy) {
  // 3.5x + y
  return
    (dx >> 1) +
    dx +
    dx +
    dx +
    (dy >> 1);
}


/// \brief
///   Updates the state of the animation
///
void CardAnimator::Service() {
  switch (state) {
  case State::Idle:
    break;

  case State::Animating:
    if (timeLeft == 0) {
      Game::instance.SetCard(endLocation, cardToMove);
      state = State::Idle;
      break;
    }
    UpdatePosition();
    GetOffscreenPage()->MoveCard(cardToMove, currentX, currentY);
    SwapPages();
    break;
  }
}


void CardAnimator::SwapPages()
{
   showingPage1 = !showingPage1;
   GetOnscreenPage()->Show();
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
  while (lastVBLCount != now) {
    numeratorX += distanceX;
    while (numeratorX >= duration) {
      currentX += directionX;
      numeratorX -= duration;
    }

    numeratorY += distanceY;
    while (numeratorY >= duration) {
      currentY += directionY;
      numeratorY -= duration;
    }

    ++lastVBLCount;
    if (--timeLeft == 0)
      break;
  }
}


/// ========================================================================
/// ========================================================================
///     class AnimationPage
/// ========================================================================
/// ========================================================================

void AnimationPage::MoveCard(CompactCard card, uint8_t x, uint8_t y)
{
   // if we have a saved background we need to restore it
   if (backgroundSaved)
   {
      drawing->RestoreBackground(&background, backgroundX, backgroundY);
      backgroundSaved = false;
   }

   // save the background at the new location
   backgroundX = x;
   backgroundY = y;
   drawing->SaveCardBackground(x, y, &background);
   backgroundSaved = true;
   drawing->DrawCardWithShadow(card, x, y);
}


void AnimationPage::DrawGame()
{
   drawing->DrawBackground();
   drawing->DrawGame();
   backgroundSaved = false;
}


void AnimationPage::CopyFrom(AnimationPage &from)
{
   from.drawing->CopyTo(drawing);
   backgroundSaved = false;
}


void AnimationPage::EraseCard(CardLocation location)
{
   drawing->EraseCard(location);
}
