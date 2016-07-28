// Ex2_02A.cpp
// Sorting strings in a vector container
#include <iostream>                         // For standard streams
#include <string>                           // For string types
#include <algorithm>                        // For swap() and copy() functions
#include <vector>                           // For vector (and iterators)
using std::string;
using std::vector;

// Our very own algorithm to sort a range specified by iterators
template<typename RandomIter>
void bubble_sort(RandomIter start, RandomIter last)
{
    std::cout << "Starting sort." << std::endl;
    bool out_of_order {false};                // true when values are not in order

    while (true) {
        for (auto first = start + 1; first != last; ++first) {
            if (*(first - 1) > *first) {
                // Out of order so swap them
                std::swap(*first, *(first - 1));
                out_of_order = true;
            }
        }

        if (!out_of_order)                      // If they are in order (no swaps necessary)...
            break;                                // ...we are done...

        out_of_order = false;                   // ...otherwise, go round again.
    }
}

int main()
{
    vector<string> words;                     // Stores words to be sorted
    words.reserve(10);                        // Allocate some space for elements
    std::cout << "Enter words separated by spaces. Enter Ctrl+Z on a separate line to end:" << std::endl;
    std::copy(std::istream_iterator < string > {std::cin}, std::istream_iterator < string > {},
              std::back_inserter(words));
    bubble_sort(std::begin(words), std::end(words));  // Sort the words array
    // Output the sorted vector
    std::cout << "your words in ascending sequence:" << std::endl;
    std::copy(std::begin(words), std::end(words), std::ostream_iterator < string > {std::cout, " "});
    std::cout << std::endl;
    // Create a new vector by moving elements from words vector
    vector<string> words_copy {std::make_move_iterator(std::begin(words)),
                               std::make_move_iterator(std::end(words))};
    std::cout << "\nAfter moving elements from words, words_copy contains:" << std::endl;
    std::copy(std::begin(words_copy), std::end(words_copy),  std::ostream_iterator < string > {std::cout, " "});
    std::cout << std::endl;
    // See what's happened to elements in words vector...
    std::cout << "\nwords vector has " << words.size() << " elements\n";

    if (words.front().empty())
        std::cout << "First element is empty string object." << std::endl;

    std::cout << "First element is \"" << words.front() << "\"" << std::endl;
}
