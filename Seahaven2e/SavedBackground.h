// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#ifndef SEAHAVEN2E_SAVEDBACKGROUND_H_
#define SEAHAVEN2E_SAVEDBACKGROUND_H_

/// <summary>
/// Memory area for saving the image behind a card during animation
/// </summary>
struct SavedBackground {
  static constexpr uint8_t Height = CardHeight + CardLocations::CardShadowHeight;

  constexpr SavedBackground() : pixels() {}

  uint8_t pixels[4 * Height];
};

#endif  // SEAHAVEN2E_SAVEDBACKGROUND_H_
