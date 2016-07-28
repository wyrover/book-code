// Ex9_02.cpp
// Using output stream iterator function members
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For iterators and begin() and end()
#include <vector>                                        // For vector container
#include <algorithm>                                     // For copy() algorithm
#include <string>
using std::string;

int main()
{
    std::vector<string> words {"The", "quick", "brown", "fox", "jumped", "over", "the", "lazy", "dog"};
    // Write the words container using conventional iterator notation
    std::ostream_iterator<string> out_iter1 {std::cout};   // Iterator with no delimiter output

    for (const auto& word : words) {
        *out_iter1++ = word;                                 // Write a word
        *out_iter1++ = " ";                                  // Write a delimiter
    }

    *out_iter1++ = "\n";                                   // Write newline

    // Write the words container again using the iterator
    for (const auto& word : words) {
        (out_iter1 = word) = " ";                            // Write the word and delimiter
    }

    out_iter1 = "\n";                                      // Write newline
    // Write the words container using copy()
    std::ostream_iterator<string> out_iter2 {std::cout, " "};
    std::copy(std::begin(words), std::end(words), out_iter2);
    out_iter2 = "\n";
}