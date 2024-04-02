// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SCREENSAVE_H
#define SCREENSAVE_H

/// <summary>
/// Enters screensave mode
/// </summary>
void ScreensaveStart();

/// <summary>
/// Performs periodic tasks during screensave mode
/// </summary>
void ScreensaveService();

#endif // SCREENSAVE_H
