// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "PersistentState.h"

#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "SHAssert.h"


/// <summary>
/// our global instance
/// </summary>
PersistentState PersistentState::instance;

// signature bytes
static const uint8_t SIGNATURE1 = 0x42;
static const uint8_t SIGNATURE2 = 0x24;

// our filename... needs to change when the format of the data changes...
// that's how we handle versioning
#define FILENAME "SHDATAV0"


static uint8_t HexDigit(uint8_t nybble);


/// <summary>
/// Loads the persistent state o=from a file
/// </summary>
void PersistentState::Load()
{
   // We have a couple signature bytes, one at the beginning and one at
   // the end, which we use as an integrity check, along with a checksum.
   // Wreck the signature bytes so we can recognize failure
   signature1 = ~SIGNATURE1;
   signature2 = ~SIGNATURE2;

   uint8_t loadAddressLo = (uint8_t)(uint16_t)this;
   uint8_t loadAddressHi = (uint8_t)(((uint16_t)this)>>8);

   // Our command line is declared as const so that it gets put into the
   // ro-data section (i.e. is initialized on program load).  But we choose
   // to treat it as mutable so that we can install our target address
   static const char BASE_LOAD_COMMAND[] = "BLOAD " FILENAME ",A$";
   char loadCommand[sizeof(BASE_LOAD_COMMAND) + 4];
   c6502::memcpy8(loadCommand, BASE_LOAD_COMMAND, sizeof(BASE_LOAD_COMMAND) - 1);
   loadCommand[sizeof(BASE_LOAD_COMMAND) - 1] = HexDigit(loadAddressHi>>4);
   loadCommand[sizeof(BASE_LOAD_COMMAND) + 0] = HexDigit(loadAddressHi&0xF);
   loadCommand[sizeof(BASE_LOAD_COMMAND) + 1] = HexDigit(loadAddressLo>>4);
   loadCommand[sizeof(BASE_LOAD_COMMAND) + 2] = HexDigit(loadAddressLo&0xF);
   loadCommand[sizeof(BASE_LOAD_COMMAND) + 3] = 0;

   // execute; it's left to the caller to request an integrity check
   a2::ExecuteDOSCommand(loadCommand);
}


bool PersistentState::CheckIntegrity() const
{
   if (signature1 != SIGNATURE1)
      return false;
   if (signature2 != SIGNATURE2)
      return false;
   if (checksum != CalculateChecksum())
      return false;

   return true;
}


uint16_t PersistentState::CalculateChecksum() const
{
   // calculate the checksum of everything
   uint16_t result = 0;
   const uint8_t *pByte = reinterpret_cast<const uint8_t *>(this);
   for (uint16_t i=0; i<sizeof(*this); ++i)
      result += *pByte++;

   // subtract the checksum bytes
   result -= (uint8_t)(checksum >> 8);
   result -= (uint8_t)checksum;
   return result;
}


void PersistentState::Reset()
{
   // start with any defaults we have defined
   *this = PersistentState();

   // deal the initial game
   Game.Shuffle16(this->gameSeed);
}


__attribute__((noinline)) static uint8_t HexDigit(uint8_t nybble)
{
   if (nybble >= 10)
      return ('A' - 10) + nybble;
   else
      return '0' + nybble;
}
