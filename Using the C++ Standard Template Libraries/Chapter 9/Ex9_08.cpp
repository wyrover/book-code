// Ex9_08.cpp
// Using stream iterators to create a file of random temperatures
#include <iostream>                                        // For standard streams
#include <iterator>                                        // For iterators and begin() and end()
#include <iomanip>                                         // For stream manipulators
#include <fstream>                                         // For file streams
#include <algorithm>                                       // For generate_n() and for_each()
#include <random>                                          // For distributions and random number generator
#include <string>                                          // For string class
using std::string;

int main()
{
    string file_name {"G:/Beginning_STL/temperatures.txt"};
    std::ofstream temps_out {file_name, std::ios_base::out | std::ios_base::trunc};
    const size_t n {50};                                       // Number of temperatures required
    std::random_device rd;                                     // Non-determistic source
    std::mt19937 rng {rd()};                                   // Mersenne twister generator
    double mu {50.0}, sigma {15.0};                            // Mean: 50 degrees SD: 15
    std::normal_distribution<> normal {mu, sigma};             // Create distribution
    // Write random temperatures to the file
    std::generate_n(std::ostream_iterator<double> { temps_out, " "}, n,
                    [&rng, &normal]
    { return normal(rng); });
    temps_out.close();                                         // Close the output file
    // List the contents of the file
    std::ifstream temps_in {file_name};                        // Open the file to read it
    for_each(std::istream_iterator<double> {temps_in}, std::istream_iterator<double> {},
    [](double t) {
        const size_t perline {10};
        static size_t count {};
        std::cout << std::fixed << std::setprecision(2) << std::setw(5) << t
                  << ((++count % perline) ? " " : "\n");
    });
    std::cout << std::endl;
    temps_in.close();                                       // Close the input file
}