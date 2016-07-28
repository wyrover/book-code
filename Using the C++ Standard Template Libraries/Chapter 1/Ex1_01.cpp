// Ex1_01.cpp
// Using iterators
#include <numeric>                                       // For accumulate() - sums a range of elements
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For iterators and begin() and end()

int main()
{
    double data[] {2.5, 4.5, 6.5, 5.5, 8.5};
    std::cout << "The array contains:\n";

    for (auto iter = std::begin(data); iter != std::end(data); ++iter)
        std::cout << *iter << " ";

    auto total = std::accumulate(std::begin(data), std::end(data), 0.0);
    std::cout << "\nThe sum of the array elements is " << total << std::endl;
}