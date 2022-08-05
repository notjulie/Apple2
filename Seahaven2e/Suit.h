/** \brief
 * Suit defintions
 */


#ifndef SUIT_H
#define SUIT_H

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
inline Suit &operator++(Suit &s) {
   s = (Suit)((uint8_t)s + 1);
   return s;
}

/** \brief
 * Suit post-increment operator
 */
inline Suit operator++(Suit &s, int) {
   s = (Suit)((uint8_t)s + 1);
   return s;
}

#endif // SUIT_H

