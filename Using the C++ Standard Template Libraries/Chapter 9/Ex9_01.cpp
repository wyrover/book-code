// Ex9_01.cpp
// Calling istream_iterator function members
#include <iostream>                                   // For standard streams
#include <iterator>                                   // For stream iterators 

int main()
{
    std::cout << "Enter some integers - enter Ctrl+Z to end.\n";
    std::istream_iterator<int> iter {std::cin};         // Create begin input stream iterator...
    std::istream_iterator<int> copy_iter {iter};        // ...and a copy
    std::istream_iterator<int> end_iter;                // Create end input stream iterator
    // Read some integers to sum
    int sum {};

    while (iter != end_iter) {                          // Continue until Ctrl+Z read
        sum += *iter++;
    }

    std::cout << "Total is " << sum << std::endl;
    std::cin.clear();                                   // Clear EOF state
    std::cin.ignore();                                  // Skip characters
    // Read integers using the copy of the iterator
    std::cout << "Enter some more integers - enter Ctrl+Z to end.\n";
    int product {1};

    while (true) {
        if (copy_iter == end_iter) break;                 // Break if Ctrl+Z was read

        product *= *copy_iter++;
    }

    std::cout << "product is " << product << std::endl;
}