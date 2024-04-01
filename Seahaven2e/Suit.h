// =============================================================
//    Copyright 2023 Randy Rasmussen
//
//    Suit definitions.
// =============================================================

#ifndef SEAHAVEN2E_SUIT_H_
#define SEAHAVEN2E_SUIT_H_

/// <summary>
/// Enum specifically intended to represent ordinal 0..3 values
/// for the suits, for indexing into arrays and such
/// </summary>
enum class SuitOrdinal : uint8_t {
   Clubs,
   Diamonds,
   Hearts,
   Spades
};

#endif  // SEAHAVEN2E_SUIT_H_
