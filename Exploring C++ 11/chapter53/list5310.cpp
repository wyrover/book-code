// Listing 53-10. Playing High-Card with set
#include <iostream>
#include <iterator>
#include <set>
#include <utility>

#include "card.hpp"
#include "randomint.hpp"

int main()
{
  typedef std::set<card, std::function<bool(card, card)>> cardset;
  cardset deck(acehigh_compare);
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
