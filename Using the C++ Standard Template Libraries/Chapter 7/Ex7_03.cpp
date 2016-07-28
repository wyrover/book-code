// Ex7_03.cpp
// Finding rearrangements of the letters in a word
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For iterators and begin() and end()
#include <string>                                        // For string class
#include <vector>                                        // For vector container
#include <algorithm>                                     // For next_permutation()
using std::string;

int main()
{
    std::vector<string> words;
    string word;

    while (true) {
        std::cout << "\nEnter a word, or Ctrl+z to end: ";

        if ((std::cin >> word).eof()) break;

        string word_copy {word};

        do {
            words.push_back(word);
            std::next_permutation(std::begin(word), std::end(word));
        } while (word != word_copy);

        size_t count{}, max{8};

        for (const auto& wrd : words)
            std::cout << wrd << ((++count % max == 0) ? '\n' : ' ');

        std::cout << std::endl;
        words.clear();                     // Remove previous permutations
    }
}