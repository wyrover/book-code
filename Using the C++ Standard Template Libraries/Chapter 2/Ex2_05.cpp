// Ex2_05.cpp
// Working with a list
#include <iostream>
#include <list>
#include <string>
#include <functional>

using std::list;
using std::string;

// List a range of elements
template<typename Iter>
void list_elements(Iter begin, Iter end)
{
    while (begin != end)
        std::cout << *begin++ << std::endl;
}

int main()
{
    std::list<string> proverbs;
    // Read the proverbs
    std::cout << "Enter a few proverbs and enter an empty line to end:" << std::endl;
    string proverb;

    while (getline(std::cin, proverb, '\n'), !proverb.empty())
        proverbs.push_front(proverb);

    std::cout << "Go on, just one more:" << std::endl;
    getline(std::cin, proverb, '\n');
    proverbs.emplace_back(proverb);
    std::cout << "The elements in the list in reverse order are:" << std::endl;
    list_elements(std::rbegin(proverbs), std::rend(proverbs));
    proverbs.sort();                               // Sort the proverbs in ascending sequence
    std::cout << "\nYour proverbs in ascending sequence are:" << std::endl;
    list_elements(std::begin(proverbs), std::end(proverbs));
    proverbs.sort(std::greater<>());               // Sort the proverbs in descending sequence
    std::cout << "\nYour proverbs in descending sequence:" << std::endl;
    list_elements(std::begin(proverbs), std::end(proverbs));
}
