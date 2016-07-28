// Ex4_03.cpp
// Stores one or more quotations for a name in a map
#include <iostream>                              // For standard streams
#include <cctype>                                // For toupper()
#include <map>                                   // For map containers
#include <string>                                // For string class
#include "Quotations.h"
#include "Name.h"

using std::string;

// Read a name from standard input
inline Name get_name()
{
    Name name {};
    std::cout << "Enter first name and second name: ";
    std::cin >> std::ws >> name;
    return name;
}

// Read a quotation from standard input
inline string get_quote(const Name& name)
{
    std::cout << "Enter the quotation for " << name
              << ". Enter * to end:\n";
    string quote;
    std::getline(std::cin >> std::ws, quote, '*');
    return quote;
}

int main()
{
    std::map<Name, Quotations> quotations;         // Container for name/quotes pairs
    std::cout << "Enter 'A' to add a quote."
              "\nEnter 'L' to list all quotes."
              "\nEnter 'G' to get a quote."
              "\nEnter 'Q' to end.\n";
    Name name {};                                  // Stores a name
    string quote {};                               // Stores a quotation
    char  command {};                              // Stores a command

    while (command != 'Q') {
        std::cout << "\nEnter command: ";
        std::cin >> command;
        command = static_cast<char>(std::toupper(command));

        switch (command) {
        case 'Q':
            break;                                     // Quit operations

        case 'A':
            name = get_name();
            quote = get_quote(name);
            quotations[name] << quote;
            break;

        case 'G': {
            name = get_name();
            const auto& quotes = quotations[name];
            size_t count = quotes.size();

            if (!count) {
                std::cout << "There are no quotes recorded for "
                          << name << std::endl;
                continue;
            }

            size_t index {};

            if (count > 1) {
                std::cout << "There are " << count << " quotes for " << name << ".\n"
                          << "Enter an index from 0 to " << count - 1 << ": ";
                std::cin >> index;
            }

            std::cout << quotations[name][index] << std::endl;
        }
        break;

        case 'L':
            if (quotations.empty()) {                                      // Test for no pairs
                std::cout << "\nNo quotations recorded for anyone." << std::endl;
            }

            // List all quotations
            for (const auto& pr : quotations) {                            // Iterate over pairs
                std::cout << '\n' << pr.first << std::endl;

                for (const auto& quote : pr.second) {                        // Iterate over quotations
                    std::cout << "  " << quote << std::endl;
                }
            }

            break;

        default:
            std::cout << " Command must be 'A', 'G', 'L', or 'Q'. Try again.\n";
            continue;
            break;
        }
    }
}