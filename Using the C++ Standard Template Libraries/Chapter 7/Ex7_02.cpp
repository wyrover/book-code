// Ex7_02.cpp
// Using the mismatch() algorithm
#include <iostream>                                      // For standard streams
#include <vector>                                        // For vector container
#include <algorithm>                                     // For equal() algorithm
#include <string>                                        // For string class
#include <iterator>                                      // For stream iterators

using std::string;
using word_iter = std::vector<string>::iterator;

int main()
{
    std::vector<string> words1 {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    std::vector<string> words2 {"two", "three", "four", "five", "six", "eleven", "eight", "nine", "ten"};
    auto iter1 = std::begin(words1);
    auto end_iter1 = std::end(words1);
    auto iter2 = std::begin(words2);
    auto end_iter2 = std::end(words2);
    // Lambda expression to output mismatch() result
    auto print_match = [](const std::pair<word_iter, word_iter>& pr, const word_iter & end_iter) {
        if (pr.first != end_iter)
            std::cout << "\nFirst pair of words that differ are "
                      << *pr.first << " and " << *pr.second << std::endl;
        else
            std::cout << "\nRanges are identical." << std::endl;
    };
    std::cout << "Container - words1:  ";
    std::copy(iter1, end_iter1, std::ostream_iterator<string> {std::cout, " "});
    std::cout << "\nContainer - words2:  ";
    std::copy(iter2, end_iter2, std::ostream_iterator<string> {std::cout, " "});
    std::cout << std::endl;
    std::cout << "\nCompare from words1[1] to end with words2:";
    print_match(std::mismatch(iter1 + 1, end_iter1, iter2), end_iter1);
    std::cout << "\nCompare from words2[0] to second-to-last with words1:";
    print_match(std::mismatch(iter2, end_iter2 - 1, iter1), end_iter2 - 1);
    std::cout << "\nCompare from words1[1] to words1[5] with words2:";
    print_match(std::mismatch(iter1 + 1, iter1 + 6, iter2), iter1 + 6);
    std::cout << "\nCompare first 6 from words1 with first 6 in words2:";
    print_match(std::mismatch(iter1, iter1 + 6, iter2, iter2 + 6), iter1 + 6);
    std::cout << "\nCompare all words1 with words2:";
    print_match(std::mismatch(iter1, end_iter1, iter2), end_iter1);
    std::cout << "\nCompare all of words2 with all of words1:";
    print_match(std::mismatch(iter2, end_iter2, iter1, end_iter1), end_iter2);
    std::cout << "\nCompare from words1[1] to end with words2[0] to second-to-last:";
    print_match(std::mismatch(iter1 + 1, end_iter1, iter2, end_iter2 - 1), end_iter1);
}
