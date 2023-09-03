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
/// Loads the persistent state from a file
/// </summary>
__attribute__((noinline)) void PersistentState::Load()
{
   // We have a couple signature bytes, one at the beginning and one at
   // the end, which we use as an integrity check, along with a checksum.
   // Wreck the signature bytes so we can recognize failure
   signature1 = ~SIGNATURE1;
   signature2 = ~SIGNATURE2;

   uint8_t loadAddressLo = (uint8_t)(uint16_t)this;
   uint8_t loadAddressHi = (uint8_t)(((uint16_t)this)>>8);

   // Format up a command
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


__attribute__((noinline)) void PersistentState::Save()
{
   // We have a couple signature bytes, one at the beginning and one at
   // the end, which we use as an integrity check, along with a checksum.
   // Set the signature bytes before writing.
   signature1 = SIGNATURE1;
   signature2 = SIGNATURE2;

   // set the checksum
   checksum = CalculateChecksum();

   uint8_t saveAddressLo = (uint8_t)(uint16_t)this;
   uint8_t saveAddressHi = (uint8_t)(((uint16_t)this)>>8);
   uint8_t saveLengthLo = (uint8_t)sizeof(*this);
   uint8_t saveLengthHi = (uint8_t)(sizeof(*this)>>8);

   // Format up a command
   static const char BASE_SAVE_COMMAND[] = "BSAVE " FILENAME ",A$";
   char saveCommand[sizeof(BASE_SAVE_COMMAND) + 11];
   c6502::memcpy8(saveCommand, BASE_SAVE_COMMAND, sizeof(BASE_SAVE_COMMAND) - 1);
   saveCommand[sizeof(BASE_SAVE_COMMAND) - 1] = HexDigit(saveAddressHi>>4);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 0] = HexDigit(saveAddressHi&0xF);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 1] = HexDigit(saveAddressLo>>4);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 2] = HexDigit(saveAddressLo&0xF);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 3] = ',';
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 4] = 'L';
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 5] = '$';
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 6] = HexDigit(saveLengthHi>>4);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 7] = HexDigit(saveLengthHi&0xF);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 8] = HexDigit(saveLengthLo>>4);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 9] = HexDigit(saveLengthLo&0xF);
   saveCommand[sizeof(BASE_SAVE_COMMAND) + 10] = 0;

   // execute; it's left to the caller to request an integrity check
   a2::puts(saveCommand);
   a2::ExecuteDOSCommand(saveCommand);
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
