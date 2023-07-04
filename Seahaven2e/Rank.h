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
/// Rank addition operator
/// </summary>
inline Rank operator+(Rank &r, int8_t i) {
  r = (Rank)((uint8_t)r + i);
  if (r<Rank::Ace || r>Rank::King)
    r = Rank::Null;
  return r;
}

/// <summary>
/// Rank subtraction operator
/// </summary>
inline Rank operator-(Rank &r, int8_t i) {
  return r + (-i);
}

#endif  // SEAHAVEN2E_RANK_H_
