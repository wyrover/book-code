// Listing 53-6. Playing High-Card with array
#include <array>
#include <iostream>

#include "card.hpp"
#include "randomint.hpp" // Listing 43-5


int main()
{
  std::array<card, 52> deck;
  std::generate(deck.begin(), deck.end(), card_generator{});

  randomint picker{0, deck.size() - 1};
  for (int i{0}; i != 10; ++i)
  {
    card const& computer_card{deck.at(picker())};
    std::cout << "I picked " << computer_card << '\n';

    card const& user_card{deck.at(picker())};
    std::cout << "You picked " << user_card << '\n';

    if (acehigh_compare(computer_card, user_card))
      std::cout << "You win.\n";
    else
      std::cout << "I win.\n";
  }
}
