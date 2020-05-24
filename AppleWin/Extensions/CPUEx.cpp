
#include "stdafx.h"
#include "Common.h"
#include "CPU.h"
#include "Memory.h"

#include "CPUEx.h"


void CPUEx::Jump(uint16_t address)
{
   regs.pc = address;
}


void CPUEx::Jsr(uint16_t address)
{
   uint16_t pushAddress = regs.pc - 1;
   Push(pushAddress >> 8);
   Push(pushAddress & 0xFF);
   Jump(address);
}


void CPUEx::Push(uint8_t value)
{
   mem[regs.sp--] = value;
   if (regs.sp < 0x100)
      regs.sp += 0x100;
}


void CPUEx::CopyToAppleMemory(const void* memory, uint16_t address, uint16_t length)
{
   std::memcpy(&mem[address], memory, length);
}
