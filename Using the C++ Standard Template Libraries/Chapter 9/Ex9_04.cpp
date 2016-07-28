// Ex9_04.cpp
// Finding anagrams of a word by re-reading the dictionary file
#include <iostream>                                      // For standard streams
#include <fstream>                                       // For file streams
#include <iterator>                                      // For iterators and begin() and end()
#include <string>                                        // For string class
#include <set>                                           // For set container
#include <vector>                                        // For vector container
#include <algorithm>                                     // For next_permutation()
using std::string;

int main()
{
    string file_in {"G:/Beginning_STL/dictionary.txt"};
    std::ifstream in {file_in};

    if (!in) {
        std::cerr << file_in << " not open." << std::endl;
        exit(1);
    }

    auto end_iter = std::istream_iterator<string> {};
    std::vector<string> words;
    string word;

    while (true) {
        std::cout << "\nEnter a word, or Ctrl+z to end: ";

        if ((std::cin >> word).eof()) break;

        string word_copy {word};

        do {
            in.seekg(0);                                              // File position at beginning

            // Use find() algorithm to read the file to check for an anagram
            if (std::find(std::istream_iterator<string>(in), end_iter, word) != end_iter)
                words.push_back(word);
            else
                in.clear();                                             // Reset EOF

            std::next_permutation(std::begin(word), std::end(word));
        } while (word != word_copy);

        std::copy(std::begin(words), std::end(words), std::ostream_iterator<string> {std::cout, " "});
        std::cout << std::endl;
        words.clear();                                              // Remove previous permutations
    }

    in.close();                                                   // Close the file
}
