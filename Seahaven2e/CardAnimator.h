// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARDANIMATOR_H_
#define SEAHAVEN2E_CARDANIMATOR_H_

#include "AnimationPage.h"
#include "Card.h"
#include "CardLocation.h"


/// <summary>
/// Class responsible for maintaining the state of our animations
/// </summary>
class CardAnimator {
public:
   CardAnimator();
   void Initialize();

   void DrawGame();
   bool IsAnimating() { return state != State::Idle; }
   void StartAnimation(CompactCard card, CardLocation end);
   void Service();

   AnimationPage GetOnscreenPage() const { return onscreenPage; }

public:
  static CardAnimator instance;

private:
  enum class State {
    /// <summary>
    /// Both HGR pages the same and up to date
    /// </summary>
    Idle,

    /// <summary>
    /// Animating
    /// </summary>
    Animating
  };

private:
   static uint8_t CalculatePixelDistance(uint8_t dx, uint8_t dy);
   void SwapPages();
   void UpdatePosition();

private:
   // operating state
   State state;

   // our animation pages
   AnimationPage offscreenPage;
   AnimationPage onscreenPage;

   // the animation in progress; relevant only in animating states
   // don't need to be cleared by constructor
   CompactCard cardToMove;
   CardLocation endLocation;
   uint8_t currentX, currentY;
   uint8_t targetX, targetY;
   uint8_t distanceX, distanceY;
   int8_t directionX, directionY;
   uint8_t numeratorX, numeratorY;
   uint8_t lastVBLCount;
   uint8_t duration;
   uint8_t timeLeft;
};

#endif  // SEAHAVEN2E_CARDANIMATOR_H_
