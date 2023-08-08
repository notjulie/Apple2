// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SCREENSAVE_H
#define SCREENSAVE_H

#include <stdint.h>

class Screensave {
public:
   void Start();
   void Service();

public:
   static Screensave instance;

private:
   enum class Edge {
      Top,
      Left,
      Bottom,
      Right
   };

private:
   void ChooseRandomTarget();
   void StartNextAnimation();

   static uint8_t GetRandomX();
   static uint8_t GetRandomY();

private:
   Edge targetEdge;
   uint8_t targetX;
   uint8_t targetY;
};

#endif // SCREENSAVE_H
