#include <iostream>
#include <boost/chrono.hpp>

namespace chrono = boost::chrono;

int main()
{
    typedef chrono::system_clock::period tick_period;
    std::cout
            << boost::ratio_string<tick_period, char>::prefix()
            << " seconds\n";
    chrono::system_clock::time_point epoch;
    chrono::system_clock::time_point now =
        chrono::system_clock::now();
    std::cout << epoch << '\n';
    std::cout << chrono::time_point_cast<chrono::hours>(now)
              << '\n';
}

