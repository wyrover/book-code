// Listing 53-5. The card Class, to Represent a Playing Card
#ifndef CARD_HPP_
#define CARD_HPP_

#include <iosfwd>

/// Represent a standard western playing card.
class card
{
public:
  typedef char suit;
  static suit const spades   {4};
  static suit const hearts   {3};
  static suit const clubs    {2};
  static suit const diamonds {1};

  typedef char rank;
  static rank const ace   {14};
  static rank const king  {13};
  static rank const queen {12};
  static rank const jack  {11};

  card() : rank_{0}, suit_{0} {}
  card(rank r, suit s) : rank_{r}, suit_{s} {}

  void assign(rank r, suit s);
  suit get_suit() const { return suit_; }
  rank get_rank() const { return rank_; }
private:
  rank rank_;
  suit suit_;
};

bool operator==(card a, card b);
bool operator!=(card a, card b);
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
