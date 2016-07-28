// Listing 65-4. Improving the card Class with Enumerations
#ifndef CARD_HPP_
#define CARD_HPP_

#include <istream>
#include <ostream>

enum class suit { nosuit, diamonds, clubs, hearts, spades };
enum class rank { norank=0, r2=2, r3, r4, r5, r6, r7, r8, r9, r10, jack, queen, king, ace };

suit& operator++(suit& s)
{
   if (s == suit::spades)
      s = suit::diamonds;
  else
     s = static_cast<suit>(static_cast<int>(s) + 1);
  return s;
}

rank operator++(rank& r)
{
   if (r == rank::norank or r == rank::ace)
      r = rank::r2;
   else
      r = static_cast<rank>(static_cast<int>(r) + 1);
   return r;
}

/// Represent a standard western playing card.
class card
{
public:
  card() : rank_(rank::norank), suit_(suit::nosuit) {}
  card(rank r, suit s) : rank_(r), suit_(s) {}

  void assign(rank r, suit s);
  suit get_suit() const { return suit_; }
  rank get_rank() const { return rank_; }
private:
  rank rank_;
  suit suit_;
};

bool operator==(card a, card b);
std::ostream& operator<<(std::ostream& out, card c);
std::istream& operator>>(std::istream& in, card& c);

/// In some games, Aces are high. In other Aces are low. Use different
/// comparison functors depending on the game.
bool acehigh_compare(card a, card b);
bool acelow_compare(card a, card b);

/// Generate successive playing cards, in a well-defined order,
/// namely, 2-10, J, Q, K, A. Diamonds first, then Clubs, Hearts, and Spades.
/// Roll-over and start at the beginning again after generating 52 cards.
class card_generator
{
public:
  card_generator();
  card operator()();
private:
  card card_;
};

#endif
