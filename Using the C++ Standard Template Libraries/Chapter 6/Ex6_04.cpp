// Ex6_04.cpp
// Using partition_copy() to find values above average and below average
#include <iostream>                              // For standard streams
#include <vector>                                // For vector container
#include <algorithm>                             // For partition_copy(), copy()
#include <numeric>                               // For accumulate()
#include <iterator>                              // For back_inserter, ostream_iterator

int main()
{
    std::vector<double> temperatures {65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50};
    std::vector<double> low_t;                       // Stores below average temperatures
    std::vector<double> high_t;                      // Stores average or above temperatures
    auto average = std::accumulate(std::begin(temperatures), std::end(temperatures), 0.0) / temperatures.size();
    std::partition_copy(std::begin(temperatures), std::end(temperatures), std::back_inserter(low_t), std::back_inserter(high_t),
    [average](double t) {
        return t < average;
    });
    // Output below average temperatures
    std::copy(std::begin(low_t), std::end(low_t), std::ostream_iterator<double> {std::cout, " "});
    std::cout << std::endl;
    // Output average or above temperatures
    std::copy(std::begin(high_t), std::end(high_t), std::ostream_iterator<double> {std::cout, " "});
    std::cout << std::endl;
}