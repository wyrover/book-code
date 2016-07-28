// Ex4_02.cpp
// Determining word frequency
#include <iostream>                               // For standard streams
#include <iomanip>                                // For stream manipulators
#include <string>                                 // For string class
#include <sstream>                                // For istringstream
#include <algorithm>                              // For replace_if() & for_each()
#include <map>                                    // For map container
#include <cctype>                                 // For isalpha()

using std::string;

int main()
{
    std::cout << "Enter some text and enter * to end:\n";
    string text_in {};
    std::getline(std::cin, text_in, '*');
    // Replace non-alphabetic characters by a space
    std::replace_if(std::begin(text_in), std::end(text_in), [](const char& ch) {
        return !isalpha(ch);
    }, ' ');
    std::istringstream text(text_in);             // Text input string as a stream
    std::istream_iterator<string> begin(text);    // Stream iterator
    std::istream_iterator<string> end;            // End stream iterator
    std::map<string, size_t> words;               // Map to store words & word counts
    size_t max_len {};                            // Maximum word length
    // Get the words, store in the map, and find maximum length
    std::for_each(begin, end, [&max_len, &words](const string & word) {
        words[word]++;
        max_len = std::max(max_len, word.length());
    });
    size_t per_line {4}, count {};

    for (const auto& w : words) {
        std::cout << std::left << std::setw(max_len + 1) << w.first
                  << std::setw(3) << std::right << w.second << "  ";

        if (++count % per_line == 0)  std::cout << std::endl;
    }

    std::cout << std::endl;
}
