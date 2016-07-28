// Ex8_08.cpp
// Demonstrating the piecewise constant distribution
#include <random>                                        // For distributions and random number generator
#include <vector>                                        // For vector container
#include <map>                                           // For map container
#include <utility>                                       // For pair type
#include <algorithm>                                     // For copy(), count(), remove()
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For stream iterators
#include <iomanip>                                       // For stream manipulators
#include <string>                                        // For string class
using std::string;

int main()
{
    std::vector<double> b {10, 20, 35, 55};                 // Intervals: 10-20, 20-35, 35-55
    std::vector<double> w {4, 10, 6};                       // Weights for the intervals
    std::piecewise_constant_distribution<> d {std::begin(b), std::end(b), std::begin(w)};
    // Output the interval boundaries and the interval probabilities
    auto intvls = d.intervals();
    std::cout << "intervals: ";
    std::copy(std::begin(intvls), std::end(intvls), std::ostream_iterator<double> {std::cout, " "});
    std::cout << "  probability densities: ";
    auto probs = d.densities();
    std::copy(std::begin(probs), std::end(probs), std::ostream_iterator<double> {std::cout, " "});
    std::cout << '\n' << std::endl;
    std::random_device rd;
    std::default_random_engine rng {rd()};
    std::map<int, size_t> results;                          //Stores and counts random values as integers

    // Generate a lot of random values...
    for (size_t i {}; i < 20000; ++i)
        ++results[static_cast<int>(std::round(d(rng)))];

    // Plot the integer values
    auto max_count = std::max_element(std::begin(results), std::end(results),
    [](const std::pair<int, size_t>& pr1, const std::pair<int, size_t>& pr2) {
        return pr1.second < pr2.second;
    })->second;
    std::for_each(std::begin(results), std::end(results),
    [max_count](const std::pair<int, size_t>& pr) {
        if (!(pr.first % 10)) // Display value if multiple of  10
            std::cout << std::setw(3) << pr.first << "-|";
        else
            std::cout << "    |";

        std::cout << std::string(pr.second * 80 / max_count, '*')
                  << '\n';
    });
}