// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef ANIMATIONPAGE_h
#define ANIMATIONPAGE_h

#include "Drawing.h"


/// <summary>
/// Wrapper around HGRPage with support for animation
/// </summary>
class AnimationPage
{
public:
   AnimationPage() {}
   void Initialize(Drawing drawing) { this->drawing = drawing; }

   void CopyFrom(AnimationPage &from);
   void DrawGame();
   void EndAnimation();
   void EraseCard(CardLocation location);
   void MoveCard(CompactCard card, uint8_t x, uint8_t y);

   void Show() { drawing.Show(); }

   Drawing GetDrawing() { return drawing; }

private:
   // construction parameters
   Drawing drawing;

   // saved background for this page; relevant only in animating states,
   // doesn't need to be cleared by constructor
   uint8_t backgroundX;
   uint8_t backgroundY;
   bool backgroundSaved;
   SavedBackground background;
};


#endif // ANIMATIONPAGE_h
