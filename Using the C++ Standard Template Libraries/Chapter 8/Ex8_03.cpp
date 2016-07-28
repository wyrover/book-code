// Ex8_03.cpp
// Dealing cards at random with a distribution
#include <iostream>                                      // For standard streams
#include <ostream>                                       // For ostream stream
#include <iomanip>                                       // For stream manipulators
#include <iterator>                                      // For iterators and begin() and end()
#include <random>                                        // For random number generators & distributions
#include <utility>                                       // For pair<T1,T2> template
#include <vector>                                        // For vector<T> container
#include <list>                                          // For list<T> container
#include <stack>                                         // For stack<T> container
#include <array>                                         // For array<T,N> container
#include <string>                                        // For string class
#include <type_traits>                                   // For is_same predicate

using std::string;
enum class Suit : size_t {Clubs, Diamonds, Hearts, Spades};
enum class Face : size_t {Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
using Card = std::pair<Suit, Face>;                      // The type of a card
using Hand = std::list<Card>;                            // Type for a hand of cards
using Deck = std::list<Card>;                            // Type for a deck of cards
using Hands = std::vector<Hand>;                         // Type for a container of hands
using Range = std::uniform_int_distribution<size_t>::param_type;

// Stream insertion operator for a Card object
std::ostream& operator<<(std::ostream& out, const Card& card)
{
    static std::array<string, 4> suits {"C", "D", "H", "S"};           // Suit names
    static std::array<string, 13> values {"2", "3", "4", "5", "6",     // Face value names
                                          "7", "8", "9", "10", "J", "Q", "K", "A"};
    string suit {suits[static_cast<size_t>(card.first)]};
    string value {values[static_cast<size_t>(card.second)]};
    return out << std::setw(2) << value << suit;
}


// Random number generator available throughout the program code
std::default_random_engine& rng()
{
    static std::default_random_engine engine {std::random_device()()};
    return engine;
}

// Uniform distribution for type int available throughout the program code
std::uniform_int_distribution<size_t>& dist()
{
    static std::uniform_int_distribution<size_t> d ;
    return d;
}

// Initialize a deck to a full set of 52 cards
Deck& init_deck(Deck& deck)
{
    static std::array<Suit, 4> suits{Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades};
    static std::array<Face, 13> values {Face::Two, Face::Three, Face::Four, Face::Five, Face::Six,
                                        Face::Seven, Face::Eight, Face::Nine, Face::Ten,
                                        Face::Jack, Face::Queen, Face::King, Face::Ace};
    deck.clear();

    for (auto && suit : suits)
        for (auto && value : values)
            deck.emplace_back(Card {suit, value});
    return deck;
}

// Deal the complete deck
void deal(Hands& hands, Deck& deck)
{
    auto d = dist();

    while (!deck.empty()) {
        for (auto && hand : hands) {
            size_t max_index = deck.size() - 1;
            d.param(Range{0, max_index});
            auto iter = std::begin(deck);
            std::advance(iter, d(rng()));
            hand.push_back(*iter);
            deck.erase(iter);
        }
    }
}

// Sort cards in hands in card sequence
void sort_hands(Hands& hands)
{
    for (auto && hand : hands)
        hand.sort([](const auto & crd1, const auto crd2) {
        return crd1.first < crd2.first ||
               (crd1.first == crd2.first && crd1.second < crd2.second);
    });
}

// Output all hands
void show_hands(const Hands& hands)
{
    for (auto && hand : hands) {
        std::copy(std::begin(hand), std::end(hand), std::ostream_iterator<Card> {std::cout, " "});
        std::cout << std::endl;
    }
}

int main()
{
    // Create the deck
    Deck deck;
    init_deck(deck);
    // Create and deal the hands
    Hands hands(4);
    deal(hands, deck);
    // Sort and show the hands
    sort_hands(hands);
    show_hands(hands);
}