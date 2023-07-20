// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>

/// <summary>
/// Dumps a decimal number to the console.  Slowly.
/// </summary>
void DisplayNumber(uint16_t number)
{
   // split, slowly, into dividend and remainder
   uint8_t dividend = 0;
   while (number > 10)
   {
      number -= 10;
      ++dividend;
   }

   // print
   if (dividend != 0)
      DisplayNumber(dividend);
   a2::COUT('0' + number);
}

/// <summary>
/// Where Seahaven goes to die
/// </summary>
[[noreturn]] void SeahavenFail(const char *filename, uint16_t lineNumber)
{
   a2::MIXEDON();

   a2::CR();
   a2::puts("ASSERT FAIL: ");
   DisplayNumber(lineNumber);
   a2::puts(",");

   while (*filename)
   {
      char c = *filename;
      if (c>='a' && c<='z')
         c &= ~0x20;
      a2::COUT(c);
      filename++;
   }

   a2::CLREOL();

   a2::MONITOR();
}

