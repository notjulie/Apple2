// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "PersistentState.h"

#include <Apple2Lib/MMIO.h>
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
static void WriteHex4(uint8_t *dest, c6502::Int16 i16);

static const uint8_t hexDigits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/// <summary>
/// Loads the persistent state from a file
/// </summary>
__attribute__((noinline)) void PersistentState::Load()
{
   // We have a couple signature bytes, one at the beginning and one at
   // the end, which we use as an integrity check, along with a checksum.
   // Wreck the signature bytes so we can recognize failure
   signature1 = ~SIGNATURE1;
   signature2 = ~SIGNATURE2;

   // the command is declared as const so that it's saved to program memory, but
   // we overwrite it anyway
   static const char BASE_LOAD_COMMAND[] = "BLOAD " FILENAME ",A$0000";
   WriteHex4((uint8_t *)&BASE_LOAD_COMMAND[sizeof(BASE_LOAD_COMMAND) - 5], this);

   // execute; it's left to the caller to request an integrity check
   a2::ExecuteDOSCommand(BASE_LOAD_COMMAND);
}


__attribute__((noinline)) void PersistentState::Save()
{
   // We have a couple signature bytes, one at the beginning and one at
   // the end, which we use as an integrity check, along with a checksum.
   // Set the signature bytes before writing.
   signature1 = SIGNATURE1;
   signature2 = SIGNATURE2;

   // set the checksum
   checksum = CalculateChecksum();

   // the command is declared as const so that it's saved to program memory, but
   // we overwrite it anyway
   static const char BASE_SAVE_COMMAND[] = "BSAVE " FILENAME ",A$0000,L$0000";
   WriteHex4((uint8_t *)&BASE_SAVE_COMMAND[sizeof(BASE_SAVE_COMMAND) - 12], this);
   WriteHex4((uint8_t *)&BASE_SAVE_COMMAND[sizeof(BASE_SAVE_COMMAND) - 5], sizeof(*this));

   // execute; it's left to the caller to request an integrity check
   a2::ExecuteDOSCommand(BASE_SAVE_COMMAND);
}


__attribute__((noinline)) bool PersistentState::CheckIntegrity() const
{
   if (signature1 != SIGNATURE1)
      return false;
   if (signature2 != SIGNATURE2)
      return false;
   if (checksum != CalculateChecksum())
      return false;

   return true;
}


/// <summary>
/// Calculates our checksum
/// </summary>
__attribute__((noinline)) uint16_t PersistentState::CalculateChecksum() const
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


/// <summary>
/// Clears the state to initial settings
/// <summary>
__attribute__((noinline)) void PersistentState::Reset()
{
   // deal the initial game
   this->gameSeed = 0;
   Game.Shuffle16(this->gameSeed);
   UndoJournal.Clear();
}


static uint8_t HexDigit(uint8_t nybble)
{
   return hexDigits[nybble & 0x0F];
}

static uint8_t HexHighDigit(uint8_t byte)
{
   return hexDigits[byte >> 4];
}

__attribute__((noinline)) static void WriteHex4(uint8_t *dest, c6502::Int16 i16)
{
   dest[0] = HexHighDigit(i16.hi>>4);
   dest[1] = HexDigit(i16.hi);
   dest[2] = HexDigit(i16.lo>>4);
   dest[3] = HexDigit(i16.lo);
}
