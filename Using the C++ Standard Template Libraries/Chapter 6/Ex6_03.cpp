// Ex6_03.cpp
// Searching using search_n() to find freezing months
#include <iostream>                              // For standard streams
#include <vector>                                // For vector container
#include <algorithm>                             // For search_n()
#include <string>                                // For string class
using std::string;

int main()
{
    std::vector<int> temperatures {65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50};
    int max_temp {32};
    int times {3};
    auto iter = std::search_n(std::begin(temperatures), std::end(temperatures), times, max_temp,
    [](double v, double max) {
        return v <= max;
    });
    std::vector<string> months {"January", "February", "March", "April", "May", "June",
                                "July", "August", "September", "October", "November", "December"};

    if (iter != std::end(temperatures))
        std::cout << "It was " << max_temp << " degrees or below for " << times
                  << " months starting in " << months[std::distance(std::begin(temperatures), iter)]
                  << std::endl;
}