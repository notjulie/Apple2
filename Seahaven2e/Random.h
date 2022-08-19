
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>


class Random {
public:
   static uint8_t GetByte();
   /*static inline void Seed(uint16_t seed){
         a = seed >> 8;
         b = (uint8_t)seed;
      }*/


private:
   static uint8_t a;
   static uint8_t b;
};

#endif // RANDOM_H
