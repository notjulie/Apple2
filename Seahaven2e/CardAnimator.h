// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARDANIMATOR_H_
#define SEAHAVEN2E_CARDANIMATOR_H_

#include "AnimationPage.h"
#include "Card.h"
#include "CardLocation.h"
#include "UndoJournal.h"


/// <summary>
/// Class responsible for maintaining the state of our animations
/// </summary>
class CardAnimator {
public:
   CardAnimator();
   void Initialize();

   void DrawGame();
   void Erase();
   bool IsAnimating() { return state != State::Idle; }
   void StartAnimation(Card card, CardLocation end);
   void StartMoveColumnToColumn(CardLocation from, CardLocation to);
   void Service();

   void StartFreeAnimation(
         Card card,
         uint8_t startX, uint8_t startY,
         uint8_t endX, uint8_t endY,
         uint8_t duration
         );
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
      /// Animating cards on the game table
      /// </summary>
      Animating,

      /// <summary>
      /// Moving multiple cards from one column to another
      /// </summary>
      MovingColumnToColumn,

      /// <summary>
      /// Doing free animation, such as during screensave
      /// </summary>
      FreeAnimating
   };

   enum class Coordinate {
      X,
      Y
   };

private:
   static uint8_t CalculatePixelDistance(uint8_t dx, uint8_t dy);
   void SwapPages();
   void UpdateAnimation();
   void UpdatePosition();
   void NextColumnToColumnMove();
   void ServiceColumnToColumnMove();
   void StartPositionTracker(uint8_t i);
   void UpdatePositionTracker(uint8_t i);

private:
   // operating state
   State state;

   // our animation pages
   AnimationPage offscreenPage;
   AnimationPage onscreenPage;

   // the animation in progress; relevant only in animating states
   // don't need to be cleared by constructor
   Card cardToMove;
   CardLocation endLocation;
   uint8_t lastVBLCount;
   uint8_t duration;
   uint8_t timeLeft;

   // our coordinate trackers
   uint8_t currentPosition[2];
   uint8_t targetPosition[2];
   uint8_t distance[2];
   uint8_t direction[2];
   uint8_t numerator[2];

   // multiple column-to-column move in progress details
   Card cardsToMove[5];
   CardLocation startLocations[5];
   CardLocation endLocations[5];
   uint8_t numberOfCardsToMove;
   uint8_t cardBeingMoved;
};

#endif  // SEAHAVEN2E_CARDANIMATOR_H_
