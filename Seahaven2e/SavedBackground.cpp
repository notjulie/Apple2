// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#include "SavedBackground.h"
#include "Apple2Lib/VBLCounter.h"

/// <summary>
/// Saves a region from an HGR page to the SavedBackground object
/// </summary>
__attribute__((noinline)) void SavedBackground::SaveCardBackground(a2::HGRPage hgr, uint8_t x, uint8_t y)
{
   a2::VBLCounter::Update();

   backgroundX = x;
   backgroundY = y;
   backgroundSaved = true;
   backgroundHGR = hgr;

   uint8_t *p = &pixels[0];
   uint8_t *row;
   for (uint8_t i=0; i < SavedBackground::Height; ++i)
   {
      row = hgr.GetByteAddress(y++, x);
      p[0] = row[0];
      p[1] = row[1];
      p[2] = row[2];
      p[3] = row[3];
      p += 4;
   }
}


__attribute__((noinline)) void SavedBackground::RestoreAndSave(a2::HGRPage hgr, uint8_t x, uint8_t y)
{
   RestoreBackground();
   SaveCardBackground(hgr, x, y);
}


/// <summary>
/// Restores a region of the HGR page from the SavedBackground object
/// </summary>
__attribute__((noinline)) void SavedBackground::RestoreBackground()
{
   if (!backgroundSaved)
      return;
   backgroundSaved = true;

   a2::VBLCounter::Update();
   uint8_t *p = &pixels[0];
   uint8_t *row;
   uint8_t y = backgroundY;

   for (uint8_t i=0; i < SavedBackground::Height; ++i)
   {
      row = backgroundHGR.GetByteAddress(y++, backgroundX);
      row[0] = p[0];
      row[1] = p[1];
      row[2] = p[2];
      row[3] = p[3];
      p += 4;
   }
}

