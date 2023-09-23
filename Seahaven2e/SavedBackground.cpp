// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#include "SavedBackground.h"
#include "Apple2Lib/VBLCounter.h"


uint8_t SavedBackground::backgroundX[2];
uint8_t SavedBackground::backgroundY[2];
bool SavedBackground::backgroundSaved[2];
SavedBackground::Pixels SavedBackground::pixels[2];

static const a2::HGRPage backgroundHGR[2] = { a2::HGRPage::HGR(), a2::HGRPage::HGR2() };


/// <summary>
/// Saves a region from an HGR page to the SavedBackground object
/// </summary>
__attribute__((noinline)) void SavedBackground::SaveCardBackground(uint8_t page, uint8_t x, uint8_t y)
{
   a2::VBLCounter::Update();

   backgroundX[page] = x;
   backgroundY[page] = y;
   backgroundSaved[page] = true;
   CopyPixels(page, true);
}


__attribute__((noinline)) void SavedBackground::RestoreAndSave(uint8_t page, uint8_t x, uint8_t y)
{
   RestoreBackground(page);
   SaveCardBackground(page, x, y);
}


/// <summary>
/// Restores a region of the HGR page from the SavedBackground object
/// </summary>
__attribute__((noinline)) void SavedBackground::RestoreBackground(uint8_t page)
{
   if (!backgroundSaved[page])
      return;
   backgroundSaved[page] = true;

   a2::VBLCounter::Update();
   CopyPixels(page, false);
}


__attribute__((noinline)) uint8_t *SavedBackground::GetPixels(uint8_t page)
{
   return page==0 ? &pixels[0][0] : &pixels[1][0];
}


void SavedBackground::CopyPixels(uint8_t page, bool save)
{
   uint8_t *from, *to;
   from = GetPixels(page);
   to = from;

   // set our context
   a2::HGRContext::page = backgroundHGR[page];
   a2::HGRContext::row = backgroundY[page];
   a2::HGRContext::byteOffset = backgroundX[page];

   // copy
   for (uint8_t i=0; i < SavedBackground::Height; ++i)
   {
      uint8_t *row = a2::HGRContext::GetByteAddress();
      if (save)
         from = row;
      else
         to = row;

      for (int8_t j=3; j>=0; --j)
         *to++ = *from++;

      a2::HGRContext::row++;
   }
}
