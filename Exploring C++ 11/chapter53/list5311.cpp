// Listing 53-11. Playing High-Card Using an Explicit Specialization of std::less
#include <functional>
#include <iostream>
#include <iterator>
#include <set>

#include "card.hpp"
#include "randomint.hpp"

namespace std
{
  template<>
  class less<card>
  {
  public:
    typedef card first_argument_type;
    typedef card second_argument_type;
    typedef bool result_type;
    bool operator()(card a, card b) const { return acehigh_compare(a, b); }
  };
}

int main()
{
  typedef std::set<card> cardset;
  cardset deck{};
  std::generate_n(std::inserter(deck, deck.begin()), 52, card_generator{});


  for (int i{0}; i != 10; ++i)
  {
    cardset::iterator pick{deck.begin()};
    std::advance(pick, randomint{0, deck.size() - 1}());
    card computer_card{*pick};
    deck.erase(pick);
    std::cout << "I picked " << computer_card << '\n';

    pick = deck.begin();
    std::advance(pick, randomint{0, deck.size() - 1}());
    card user_card{*pick};
    deck.erase(pick);
    std::cout << "You picked " << user_card << '\n';

    if (acehigh_compare(computer_card, user_card))
      std::cout << "You win.\n";
    else
      std::cout << "I win.\n";

  }
}
