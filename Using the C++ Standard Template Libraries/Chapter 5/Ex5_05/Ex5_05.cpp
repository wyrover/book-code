// Ex5_05.cpp
// Storing pointers to derived class objects in a multiset container
#include <iostream>                              // For standard streams
#include <string>                                // For string class
#include <algorithm>                             // For copy() algorithm
#include <iterator>                              // For ostream_iterator
#include <map>                                   // For map container
#include <set>                                   // For multiset container
#include <memory>                                // For smart pointers
#include <cctype>                                // For toupper()
#include "Pet_Classes.h"

using std::string;
using Name = string;
using Pet_ptr = std::shared_ptr<Pet>;            // A smart pointer to a pet
using Pets = std::multiset <Pet_ptr>;            // A set of smart pointers to pets

// Compare shared pointers to pets
inline bool operator<(const Pet_ptr& p1, const Pet_ptr& p2)
{
    return *p1 < *p2;
}

// Stream insertion operator for pointers to pets
inline std::ostream& operator<<(std::ostream& out, const Pet_ptr& pet_ptr)
{
    return out << " " << *pet_ptr;
}

// Read in all the pets for a person
Pets get_pets(const Name& person)
{
    Pets pets;
    std::cout << "Enter " << person << "'s pets:\n";
    char ch {};
    Name name {};

    while (true) {
        std::cin >> ch;

        if (toupper(ch) == 'Q') break;

        std::cin >> name;

        switch (std::toupper(ch)) {
        case 'C':
            pets.insert(std::make_shared<Cat>(name));
            break;

        case 'D':
            pets.insert(std::make_shared<Dog>(name));
            break;

        case 'M':
            pets.insert(std::make_shared<Mouse>(name));
            break;

        default:
            std::cout << "Invalid pet ID - try again.\n";
        }
    }

    return pets;
}

// REad all the pets for a given person
void list_pets(const std::pair<Name, Pets>& pr)
{
    std::cout << "\n" << pr.first << ":\n";
    std::copy(std::begin(pr.second), std::end(pr.second), std::ostream_iterator<Pet_ptr>(std::cout, "\n"));
}

int main()
{
    std::map<Name, Pets> peoples_pets;             // The people and their pets
    char answer {'Y'};
    string name {};
    std::cout << "You'll enter a person's name followed by their pets.\n"
              << "Pets can be identified by C for cat, D for dog, or M for mouse.\n"
              << "Enter the character to identify each pet type followed by the pet's name.\n"
              << "Enter Q to end pet input for a person.\n";

    while (std::toupper(answer) == 'Y') {
        std::cout << "Enter a name: ";
        std::cin >> name;
        peoples_pets.emplace(name, get_pets(name));
        std::cout << "Another person(Y or N)? ";
        std::cin >> answer;
    }

    // Output the pets for everyone
    std::cout << "\nThe people and their pets are:\n";

    for (const auto& pr : peoples_pets)
        list_pets(pr);
}