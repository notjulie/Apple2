// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_RANK_H_
#define SEAHAVEN2E_RANK_H_

#include <stdint.h>

/** \brief
 * Rank enumeration
 */
enum class Rank {
  Null,
  Ace,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  Jack,
  Queen,
  King
};


/// <summary>
/// Rank pre-increment operator
/// </summary>
inline Rank &operator++(Rank &r) {
  r = (Rank)((uint8_t)r + 1);
  return r;
}

/// <summary>
/// Rank post-increment operator
/// </summary>
inline Rank operator++(Rank &r, int) {
  r = (Rank)((uint8_t)r + 1);
  return r;
}

#endif  // SEAHAVEN2E_RANK_H_
