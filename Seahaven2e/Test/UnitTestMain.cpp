// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/DOS.h>
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "CardAnimator.h"
#include "SHAssert.h"

static void TestCalculatePixelDistance();


/// <summary>
/// Main entry point
/// </summary>
extern "C" int main()
{
   TestCalculatePixelDistance();
   return 0;
}



static void CalculatePixelDistanceTestCase(uint8_t dx, uint8_t dy, uint8_t expectedResult)
{
   uint8_t result = CardAnimator::CalculatePixelDistance(dx, dy);
   if (result != expectedResult)
   {
      a2::puts("CALCPIXELDISTANCE ERROR: \n");
      a2::PRBYTE(dx);
      a2::puts(" ");
      a2::PRBYTE(dy);
      a2::puts(" ");
      a2::PRBYTE(expectedResult);
      a2::puts(" ");
      a2::PRBYTE(result);
      a2::puts("\n");
   }
}

static void TestCalculatePixelDistance()
{
   CalculatePixelDistanceTestCase(0, 0, 0);
   CalculatePixelDistanceTestCase(10, 0, 35);
   CalculatePixelDistanceTestCase(0, 100, 50);
   CalculatePixelDistanceTestCase(20, 100, 95);
   CalculatePixelDistanceTestCase(0, 192, 96);
   CalculatePixelDistanceTestCase(20, 192, 131);
}


extern "C" void exit()
{
   a2::PAGE2OFF();
   a2::TEXTON();
   a2::DOSWRM();
}
