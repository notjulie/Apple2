
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
   Push(regs.pc >> 8);
   Push(regs.pc & 0xFF);
   Jump(address);
}


void CPUEx::Push(uint8_t value)
{
   mem[0x100 + regs.sp--] = value;
}


void CPUEx::CopyToAppleMemory(const void* memory, uint16_t address, uint16_t length)
{
   std::memcpy(&mem[address], memory, length);
}
