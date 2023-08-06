// =============================================================
//    Copyright 2023 Randy Rasmussen
//
//    Suit definitions.
// =============================================================

#ifndef SEAHAVEN2E_SUIT_H_
#define SEAHAVEN2E_SUIT_H_

/// <summary>
/// Suit representation, very rigorously defined for the sake
/// of being able to optimize.  The point here is that we have:
///  - class Suit: the abstract representation
///  - Suit::NumericValue; the value that is ORed into class Card
///  - ordinal: value 0..3 for using the suit as an index
/// This class is supposed to facilitate using whichever is most appropriate
/// for the situation and to make sure that we always know which
/// form we are using.
/// </summary>
class Suit {
public:
   // construct as uninitialized
   Suit() {}

   constexpr uint8_t GetOrdinal() const { return ((uint8_t)numericValue)>>4; }

   bool operator==(Suit suit) const { return numericValue == suit.numericValue; }
   bool operator!=(Suit suit) const { return numericValue != suit.numericValue; }

public:
   static Suit FromOrdinal(uint8_t ordinal) {
      Suit result;
      result.numericValue = (NumericValue)(ordinal << 4);
      return result;
   }
   static constexpr Suit Clubs() { return Suit(NumericValue::Clubs); }
   static constexpr Suit Diamonds() { return Suit(NumericValue::Diamonds); }
   static constexpr Suit Hearts() { return Suit(NumericValue::Hearts); }
   static constexpr Suit Spades() { return Suit(NumericValue::Spades); }

private:
   /// <summary>
   /// our internal numeric values; these are all stored in the high
   /// nybble for the purpose of easily ORing them with the rank to
   /// represent a card
   /// </summary>
   enum class NumericValue : uint8_t {
      Clubs = 0x00,
      Diamonds = 0x10,
      Hearts = 0x20,
      Spades = 0x30
   };

private:
   constexpr Suit(NumericValue value) : numericValue(value) {}

private:
   NumericValue numericValue;
};
static_assert(sizeof(Suit)==1, "Suit should be a byte");

#endif  // SEAHAVEN2E_SUIT_H_
