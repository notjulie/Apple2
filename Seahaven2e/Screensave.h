// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SCREENSAVE_H
#define SCREENSAVE_H

class Screensave {
public:
   void Start();
   void Service();

   static Screensave instance;
};

#endif // SCREENSAVE_H
