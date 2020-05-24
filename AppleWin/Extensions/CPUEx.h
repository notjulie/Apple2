
#ifndef CPUEX_H
#define CPUEX_H

#include "stdint.h"

class CPUEx {
public:
   static void CopyToAppleMemory(const void* memory, uint16_t address, uint16_t length);
   static void Jump(uint16_t address);
   static void Jsr(uint16_t address);
   static void Push(uint8_t value);
};

#endif
