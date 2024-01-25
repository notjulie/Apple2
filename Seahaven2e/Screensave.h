// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SCREENSAVE_H
#define SCREENSAVE_H

#include <stdint.h>
#include "Card.h"


/// <summary>
/// State machine that handles our screensave mode
/// </summary>
class Screensave {
public:
   void Start();
   void Service();

public:
   static Screensave instance;

private:
   void ChooseRandomTarget();
   void StartNextAnimation();

private:
   Card cardInMotion;
   uint8_t startX, startY;
   uint8_t targetX;
   uint8_t targetY;
};

#endif // SCREENSAVE_H
