// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SHUFFLER_H
#define SHUFFLER_H

#include "Card.h"


/// <summary>
/// Class that generates a pseudo-random deck based on an integer
/// seed (aka instruction)
/// </summary>
class Shuffler {
public:
   void Shuffle16(uint16_t instruction);

public:
   // this is public... it's our output, and we don't care
   // what anyone does with it
   Card deck[52];

private:
   void Shuffle8(uint8_t instruction);
};


#endif // SHUFFLER_H
