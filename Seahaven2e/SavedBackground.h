// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_SAVEDBACKGROUND_H_
#define SEAHAVEN2E_SAVEDBACKGROUND_H_

#include <stdint.h>
#include <Apple2Lib/HGRPage.h>
#include "CardLocation.h"

/// <summary>
/// Memory area for saving the image behind a card during animation
/// </summary>
class SavedBackground
{
public:
   static void SaveCardBackground(uint8_t page, uint8_t x, uint8_t y);
   static void RestoreBackground(uint8_t page);
   static void Forget(uint8_t page) { backgroundSaved[page] = false; }
   static void RestoreAndSave(uint8_t page, uint8_t x, uint8_t y);

private:
   static uint8_t *GetPixels(uint8_t page);
   static void SavePixels(uint8_t page);
   static void RestorePixels(uint8_t page);

private:
   static constexpr uint8_t Height = CardHeight + CardLocations::CardShadowHeight;
   typedef uint8_t Pixels[4 * Height];

private:
   static uint8_t backgroundX[2];
   static uint8_t backgroundY[2];
   static bool backgroundSaved[2];
   static Pixels pixels[2];
};

#endif  // SEAHAVEN2E_SAVEDBACKGROUND_H_
