#include <string>
#include <map>
#include <list>
#include <deque>
#include <boost/assign.hpp>
#include <iostream>
#include <boost/tuple/tuple.hpp>

using namespace boost::assign;

int main()
{
    std::deque<std::string>greets;
    push_front(greets) = "Good night", "Buenas noches",
    "Bounanotte", "Lyla tov", "Gute nacht", "Bonne nuit";
    std::map<std::string, std::string> rockCharacters;
    insert(rockCharacters)
    ("John Barleycorn", "must die")       // Traffic
    ("Eleanor Rigby", "lives in a dream") // Beatles
    ("Arnold Layne", "had a strange hobby")   // Floyd
    ("Angie", "can't say we never tried")    // Stones
    ("Harry", "play the honkytonk"); // Dire Straits
    std::list<boost::tuple<std::string, std::string,
        std::string>> trios;
    push_back(trios)("Athos", "Porthos", "Aramis")
    ("Potter", "Weasley", "Granger")
    ("Tintin", "Snowy", "Haddock")
    ("Geller", "Bing", "Tribbiani")
    ("Jones", "Crenshaw", "Andrews");
    std::cout << "Night greets:\n";

    for (const auto& greet : greets) {
        std::cout << greet << '\n';
    }

    std::cout << "\nPeople:\n";

    for (const auto&character : rockCharacters) {
        std::cout << character.first << ": "
                  << character.second << '\n';
    }

    std::cout << "Trios:\n";

    for (auto& trio : trios) {
        std::cout << boost::get<0>(trio) << ", "
                  << boost::get<1>(trio) << ", "
                  << boost::get<2>(trio) << '\n';
    }
}

