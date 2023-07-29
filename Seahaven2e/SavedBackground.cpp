// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================


#include "SavedBackground.h"
#include "Apple2Lib/VBLCounter.h"

/// <summary>
/// Saves a region from an HGR page to the SavedBackground object
/// </summary>
void SavedBackground::SaveCardBackground(a2::HGRPage hgr, uint8_t x, uint8_t y)
{
  a2::VBLCounter::Update();
  uint8_t *p = &pixels[0];
  uint8_t *row;
  for (uint8_t i=0; i < SavedBackground::Height; ++i) {
    row = hgr.GetByteAddress(y++, x);
    p[0] = row[0];
    p[1] = row[1];
    p[2] = row[2];
    p[3] = row[3];
    p += 4;
  }
}


/// <summary>
/// Restores a region of the HGR page from the SavedBackground object
/// </summary>
void SavedBackground::RestoreBackground(a2::HGRPage hgr, uint8_t x, uint8_t y)
{
  a2::VBLCounter::Update();
  uint8_t *p = &pixels[0];
  uint8_t *row;
  for (uint8_t i=0; i < SavedBackground::Height; ++i) {
    row = hgr.GetByteAddress(y++, x);
    row[0] = p[0];
    row[1] = p[1];
    row[2] = p[2];
    row[3] = p[3];
    p += 4;
  }
}

