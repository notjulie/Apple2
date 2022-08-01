
#include "Drawing.h"
#include <stdint.h>
#include <Apple2Lib/HGR.h>

void DrawRectangle(uint8_t byteOffset)
{
   uint8_t row = 10;
   {
      uint8_t *rowPointer = a2::HGRPage1Base() + a2::HGRRow::GetIndex(row++) + byteOffset;
      rowPointer[0] = 0xFC;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x1F;
   }

   for (uint8_t i=0; i<37; ++i)
   {
      uint8_t *rowPointer = a2::HGRPage1Base() + a2::HGRRow::GetIndex(row++) + byteOffset;
      rowPointer[0] = 0xFE;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x3F;
   }

   {
      uint8_t *rowPointer = a2::HGRPage1Base() + a2::HGRRow::GetIndex(row++) + byteOffset;
      rowPointer[0] = 0xFC;
      rowPointer[1] = 0xFF;
      rowPointer[2] = 0xFF;
      rowPointer[3] = 0x1F;
   }
}

void DrawABunchOfRectangles()
{
   for (uint8_t offset=0; offset<40; offset+=4)
      DrawRectangle(offset);
}

