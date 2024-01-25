// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SCREENSAVE_H
#define SCREENSAVE_H

#include <stdint.h>
#include "Card.h"

class Screensave {
public:
   void Start();
   void Service();

public:
   static Screensave instance;

private:
   void ChooseRandomTarget(uint8_t startX, uint8_t startY);
   void StartNextAnimation(uint8_t startX, uint8_t startY);

private:
   Card cardInMotion;
   uint8_t targetX;
   uint8_t targetY;
};

#endif // SCREENSAVE_H
