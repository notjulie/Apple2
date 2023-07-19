// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "PersistentState.h"


/// <summary>
/// PersistentState global instance; forced into .data so that it gets
/// initialized in the output file
/// </summary>
__declspec(allocate(".data")) PersistentState PersistentState::instance;
