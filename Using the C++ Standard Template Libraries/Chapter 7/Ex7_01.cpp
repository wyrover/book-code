// Ex7_01.cpp
// Using the equal() algorithm
#include <iostream>                                      // For standard streams
#include <vector>                                        // For vector container
#include <algorithm>                                     // For equal() algorithm
#include <iterator>                                      // For stream iterators
#include <string>                                        // For string class
using std::string;

int main()
{
    std::vector<string> words1 {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    std::vector<string> words2 {"two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    auto iter1 = std::begin(words1);
    auto end_iter1 = std::end(words1);
    auto iter2 = std::begin(words2);
    auto end_iter2 = std::end(words2);
    std::cout << "Container - words1:  ";
    std::copy(iter1, end_iter1, std::ostream_iterator<string> {std::cout, " "});
    std::cout << "\nContainer - words2:  ";
    std::copy(iter2, end_iter2, std::ostream_iterator<string> {std::cout, " "});
    std::cout << std::endl;
    std::cout << "\n1. Compare from words1[1] to end with words2:                              ";
    std::cout << std::boolalpha << std::equal(iter1 + 1, end_iter1, iter2) << std::endl;
    std::cout << "2. Compare from words2[0] to second-to-last with words1:                   ";
    std::cout << std::boolalpha << std::equal(iter2, end_iter2 - 1, iter1) << std::endl;
    std::cout << "3. Compare from words1[1] to words1[5] with words2:                        ";
    std::cout << std::boolalpha << std::equal(iter1 + 1, iter1 + 6, iter2) << std::endl;
    std::cout << "4. Compare first 6 from words1 with first 6 in words2:                     ";
    std::cout << std::boolalpha << std::equal(iter1, iter1 + 6, iter2, iter2 + 6) << std::endl;
    std::cout << "5. Compare all words1 with words2:                                         ";
    std::cout << std::boolalpha << std::equal(iter1, end_iter1, iter2) << std::endl;
    std::cout << "6. Compare all of words1 with all of words2:                               ";
    std::cout << std::boolalpha << std::equal(iter1, end_iter1, iter2, end_iter2) << std::endl;
    std::cout << "7. Compare from words1[1] to end with words2 from first to second-to-last: ";
    std::cout << std::boolalpha << std::equal(iter1 + 1, end_iter1, iter2, end_iter2 - 1) << std::endl;
}