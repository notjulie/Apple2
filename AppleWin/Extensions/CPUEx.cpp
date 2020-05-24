
#include "stdafx.h"
#include "Common.h"
#include "CPU.h"
#include "CPUEx.h"


void CPUEx::Jump(uint16_t address)
{
   regs.pc = address;
}

