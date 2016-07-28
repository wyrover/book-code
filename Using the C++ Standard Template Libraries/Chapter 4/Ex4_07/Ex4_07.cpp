// Ex4_07.cpp
#include <iostream>                              // For standard streams
#include <cctype>                                 // For toupper()
#include <string>                                // For string class
#include <unordered_map>                         // For unordered_map container

#include "Record_IO.h"
#include "My_Templates.h"
#include "Hash_Function_Objects.h"

using std::string;
using std::unordered_multimap;
using Name = std::pair < string, string >;
using Phone = std::tuple < string, string, string >;

// Display command prompt
void show_operations()
{
    std::cout << "Operations:\n"
              << "A: Add an element.\n"
              << "D: Delete elements.\n"
              << "F: Find elements.\n"
              << "L: List all elements.\n"
              << "Q: Quit the program.\n\n";
}

int main()
{
    unordered_multimap<Name, Phone, NameHash> by_name {8, NameHash()};
    unordered_multimap<Phone, Name, PhoneHash> by_number {8, PhoneHash()};
    show_operations();
    char choice {};                                     // Operation selection
    Phone number {};                                    // Records a number
    Name name {};                                       // Records a name

    while (std::toupper(choice) != 'Q') {               // Go until you quit...
        std::cout << "Enter a command: ";
        std::cin >> choice;

        switch (std::toupper(choice)) {
        case 'A':                                         // Add a record
            std::cout << "Enter first & second names, area code, exchange, and number separated by spaces:\n";
            std::cin >> name >> number;
            by_name.emplace(name, number);                  // Create in place...
            by_number.emplace(number, name);                // ...in both containers
            break;

        case 'D': {                                       // Delete records
            std::cout << "Enter a name: ";                  // Only find by name
            auto pr = find_elements(by_name);
            auto count = std::distance(pr.first, pr.second);  // Number of elements

            if (count == 1) {
                // If there's just the one...
                by_number.erase(pr.first->second);            // ...delete from numbers container
                by_name.erase(pr.first);                      // ...delete from names container
            } else if (count > 1) {
                // There's more than one
                std::cout << "There are " << count << " records for "
                          << pr.first->first << ". Delete all(Y or N)? ";
                std::cin >> choice;

                if (std::toupper(choice) == 'Y') {
                    // Erase records from by_number container first
                    for (auto iter = pr.first; iter != pr.second; ++iter) {
                        by_number.erase(iter->second);
                    }

                    by_name.erase(pr.first, pr.second);         // Now delete from by_name
                }
            }
        }
        break;

        case 'F':                                         // Find a record
            std::cout << "Find by name(Y or N)? ";
            std::cin >> choice;

            if (std::toupper(choice) == 'Y') {
                std::cout << "Enter first name and second name: ";
                list_range(find_elements(by_name));
            } else {
                std::cout << "Enter area code, exchange, and number separated by spaces: ";
                list_range(find_elements(by_number));
            }

            break;

        case 'L':                                         //List all records
            std::cout << "List by name(Y or N)? ";
            std::cin >> choice;

            if (std::toupper(choice) == 'Y')
                list_elements(by_name);
            else
                list_elements(by_number);

            break;

        case 'Q':
            break;

        default:
            std::cout << "Invalid command - try again.\n";
        }
    }
}