// =============================================================
//    Copyright 2023 Randy Rasmussen
//
//    Suit definitions.
// =============================================================

#ifndef SEAHAVEN2E_SUIT_H_
#define SEAHAVEN2E_SUIT_H_

/** \brief
 * Suit enumeration
 */
enum class Suit {
  Clubs,
  Diamonds,
  Hearts,
  Spades
};

/** \brief
 * Suit pre-increment operator
 */
inline Suit operator++(Suit s) {
  s = (Suit)((uint8_t)s + 1);
  return s;
}

/** \brief
 * Suit post-increment operator
 */
inline Suit operator++(Suit s, int) {
  s = (Suit)((uint8_t)s + 1);
  return s;
}

#endif  // SEAHAVEN2E_SUIT_H_
