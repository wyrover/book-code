// Ex 6_05.cpp
// Using partition() and equal_range() to find duplicates of a value in a range
#include <iostream>                              // For standard streams
#include <list>                                  // For list container
#include <algorithm>                             // For copy(), partition()
#include <iterator>                              // For ostream_iterator

int main()
{
    std::list<int> values {17, 11, 40, 13, 22, 54, 48, 70, 22, 61, 82, 78, 22, 89, 99, 92, 43};
    // Output the elements in their original order
    std::cout << "The elements in the original sequence are:\n";
    std::copy(std::begin(values), std::end(values), std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;
    int wanted {22};                                         // What we are looking for
    std::partition(std::begin(values), std::end(values),     // Partition the values with (value < wanted)
    [wanted](double value) {
        return value < wanted;
    });
    std::partition(std::begin(values), std::end(values), // Partition the values with !(wanted < value)
    [wanted](double value) {
        return !(wanted < value);
    });
    // Output the elements after partitioning
    std::cout << "The elements after partitioning are:\n";
    std::copy(std::begin(values), std::end(values), std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;
    auto pr = std::equal_range(std::begin(values), std::end(values), wanted);
    std::cout << "The lower bound for " << wanted << " is " << *pr.first << std::endl;
    std::cout << "The upper bound for " << wanted << " is " << *pr.second << std::endl;
    std::cout << "\nThe elements found by equal_range() are:\n";
    std::copy(pr.first, pr.second, std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;
}