// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#include "SavedBackground.h"
#include <C6502/Memcpy2D.h>
#include <Apple2Lib/HGRContext.h>
#include <Apple2Lib/VBLCounter.h>

using c6502::Memcpy2D;


uint8_t SavedBackground::backgroundX[2];
uint8_t SavedBackground::backgroundY[2];
bool SavedBackground::backgroundSaved[2];
SavedBackground::Pixels SavedBackground::pixels[2];

static const a2::HGRPage backgroundHGR[2] = { a2::HGRPage::HGR(), a2::HGRPage::HGR2() };


static void SetDestByteAddressAndIncrementRow();
static void SetSourceByteAddressAndIncrementRow();


/// <summary>
/// Saves a region from an HGR page to the SavedBackground object
/// </summary>
__attribute__((noinline)) void SavedBackground::SaveCardBackground(uint8_t page, uint8_t x, uint8_t y)
{
   a2::VBLCounter::Update();

   backgroundX[page] = x;
   backgroundY[page] = y;
   backgroundSaved[page] = true;
   SavePixels(page);
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
   RestorePixels(page);
}


__attribute__((noinline)) uint8_t *SavedBackground::GetPixels(uint8_t page)
{
   return page==0 ? &pixels[0][0] : &pixels[1][0];
}



void SavedBackground::SavePixels(uint8_t page)
{
   // set our context
   a2::HGRContext::page = backgroundHGR[page];
   a2::HGRContext::row = backgroundY[page];
   a2::HGRContext::byteOffset = backgroundX[page];

   // set the source function and call it to load the first row
   Memcpy2D::SetSourceFunction(SetSourceByteAddressAndIncrementRow);
   SetSourceByteAddressAndIncrementRow();

   Memcpy2D::SetDestPointer(GetPixels(page));
   Memcpy2D::SetDestFunction(Memcpy2D::IncrementDest);

   Memcpy2D::Copy(SavedBackground::Height, 4);
}


void SavedBackground::RestorePixels(uint8_t page)
{
   // set our context
   a2::HGRContext::page = backgroundHGR[page];
   a2::HGRContext::row = backgroundY[page];
   a2::HGRContext::byteOffset = backgroundX[page];

   // set the dest function and call it to load the first row
   Memcpy2D::SetDestFunction(SetDestByteAddressAndIncrementRow);
   SetDestByteAddressAndIncrementRow();

   Memcpy2D::SetSourcePointer(GetPixels(page));
   Memcpy2D::SetSourceFunction(Memcpy2D::IncrementSource);

   Memcpy2D::Copy(SavedBackground::Height, 4);
}


static void SetDestByteAddressAndIncrementRow()
{
   Memcpy2D::SetDestPointer(a2::HGRContext::GetByteAddress());
   a2::HGRContext::row++;
}

static void SetSourceByteAddressAndIncrementRow()
{
   Memcpy2D::SetSourcePointer(a2::HGRContext::GetByteAddress());
   a2::HGRContext::row++;
}
