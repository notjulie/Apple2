
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

/** \brief
 * constexpr-friendly 256-entry pseudo-random table
 */
class RandomTable {
public:
   constexpr RandomTable()
      : table()
   {
      uint32_t r = 1111111L;
      for (int i=0; i<256; ++i)
      {
         r *= 1664525L;
         r += 1013904223L;
         table[i] = (uint8_t)r;
      }
   }

public:
   uint8_t table[256];
};

class Random {
public:
   static uint8_t GetByte() { return table.table[index++]; }
   static void Reset() { index = 0; }

private:
   static uint8_t index;
   static const RandomTable table;
};

#endif // RANDOM_H
