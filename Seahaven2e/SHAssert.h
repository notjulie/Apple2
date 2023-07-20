// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SHASSERT_H
#define SHASSERT_H

[[noreturn]] void SeahavenFail(const char *filename, uint16_t lineNumber);

#ifdef DEBUG
   #define assert(condition) { if (!(condition)) SeahavenFail(__FILE__, __LINE__); }
#else
   #define assert(condition)
#endif

#endif
