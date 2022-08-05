
#ifndef RANK_H
#define RANK_H

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

/** \brief
 * Rank pre-increment operator
 */
inline Rank &operator++(Rank &r) {
   r = (Rank)((uint8_t)r + 1);
   return r;
}

/** \brief
 * Rank post-increment operator
 */
inline Rank operator++(Rank &r, int) {
   r = (Rank)((uint8_t)r + 1);
   return r;
}

#endif
