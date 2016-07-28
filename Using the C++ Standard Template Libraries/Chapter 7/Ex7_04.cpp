// Ex7_04.cpp
// Testing for palindromes using reverse_copy()
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For stream iterators and begin() and end()
#include <algorithm>                                     // For reverse_copy() and copy_if()
#include <cctype>                                        // For toupper() and isalpha()
#include <string>
using std::string;

int main()
{
    while (true) {
        string sentence;
        std::cout << "Enter a sentence or Ctrl+Z to end: ";
        std::getline(std::cin, sentence);

        if (std::cin.eof()) break;

        // Copy as long as the characters are alphabetic & convert to upper case
        string only_letters;
        std::copy_if(std::begin(sentence), std::end(sentence), std::back_inserter(only_letters),
        [](char ch) {
            return std::isalpha(ch);
        });
        std::for_each(std::begin(only_letters), std::end(only_letters), [](char& ch) {
            ch = toupper(ch);
        });
        // Make a reversed copy
        string reversed;
        std::reverse_copy(std::begin(only_letters), std::end(only_letters), std::back_inserter(reversed));
        std::cout << '"' << sentence << '"'
                  << (only_letters == reversed ? " is" : " is not") << " a palindrome." << std::endl;
    }
}