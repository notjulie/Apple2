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
struct SavedBackground {
   static constexpr uint8_t Height = CardHeight + CardLocations::CardShadowHeight;

   void SaveCardBackground(a2::HGRPage hgr, uint8_t x, uint8_t y);
   void RestoreBackground(a2::HGRPage hgr, uint8_t x, uint8_t y);

   uint8_t pixels[4 * Height];
};

#endif  // SEAHAVEN2E_SAVEDBACKGROUND_H_
