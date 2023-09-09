// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef TEXTSCREEN_H
#define TEXTSCREEN_H

#include <Apple2Lib/TextPage.h>

class TextScreen
{
public:
   void Initialize();
   void ShowSplash();

public:
   static TextScreen instance;

private:
   a2::TextPage textPage;
};

#endif // TEXTSCREEN_H
