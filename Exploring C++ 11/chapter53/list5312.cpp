// Listing 53-12. Playing High-Card with unordered_set
#include <functional>
#include <iostream>
#include <iterator>
#include <unordered_set>

#include "card.hpp"
#include "randomint.hpp"

namespace std
{
  template<>
  class hash<card>
  {
  public:
    std::size_t operator()(card a)
    const
    {
      return hash<card::suit>{}(a.get_suit()) * hash<card::rank>{}(a.get_rank());
    }
  };
} // namespace std

int main()
{
  typedef std::unordered_set<card> cardset;
  cardset deck{};
  std::generate_n(std::inserter(deck, deck.begin()), 52, card_generator{});


  for (int i(0); i != 10; ++i)
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
