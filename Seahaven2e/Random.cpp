
#include "Random.h"

uint8_t Random::a;
uint8_t Random::b;

/** \brief
 * A really cheap RNG; it's not meant to be cryptographically random,
 * it's just good enough to take a seed value and generate a sequence
 * based off of it that's scattered enough to shuffle a deck of cards.
 * That means the user can play 64K games without repeating any.
 */
uint8_t Random::GetByte()
{
   b += 37;
   return b ^ a;
}

/** \brief
 * Seeds our generator
 */
/*void Random::Seed(uint16_t seed)
{
   a = seed >> 8;
   b = (uint8_t)seed;
}*/
