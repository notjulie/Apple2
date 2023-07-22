// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARDANIMATOR_H_
#define SEAHAVEN2E_CARDANIMATOR_H_

#include "Card.h"
#include "CardLocation.h"
#include "Drawing.h"
#include "SavedBackground.h"


/// <summary>
/// Wrapper around HGRPage with support for animation
/// </summary>
class AnimationPage
{
public:
   AnimationPage(Drawing *drawing) : drawing(drawing) {}

   void CopyFrom(AnimationPage &from);
   void DrawGame();
   void EraseCard(CardLocation location);
   void MoveCard(CompactCard card, uint8_t x, uint8_t y);

   void Show() { drawing->Show(); }

   Drawing *GetDrawing() { return drawing; }

private:
   // construction parameters
   Drawing *drawing;

   // saved background for this page; relevant only in animating states,
   // doesn't need to be cleared by constructor
   uint8_t backgroundX;
   uint8_t backgroundY;
   bool backgroundSaved;
   SavedBackground background;
};


/// <summary>
/// Class responsible for maintaining the state of our animations
/// </summary>
class CardAnimator {
public:
   CardAnimator();

   void DrawGame();
   bool IsAnimating() { return state != State::Idle; }
   void StartAnimation(CompactCard card, CardLocation end);
   void Service();

   AnimationPage *GetOnscreenPage() { return showingPage1 ? &page1 : &page2; }

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
   AnimationPage *GetOffscreenPage() { return showingPage1 ? &page2 : &page1; }
   void SwapPages();
   void UpdatePosition();

private:
   // operating state... needs to be initialized
   // by the constructor
   State state = State::Idle;
   bool showingPage1 = true;

   // our animation page 1 and 2, initialized by constructor
   AnimationPage page1 = AnimationPage(&drawing1);
   AnimationPage page2 = AnimationPage(&drawing2);

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
