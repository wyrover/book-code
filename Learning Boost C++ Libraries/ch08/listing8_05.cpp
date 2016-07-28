#include <boost/date_time.hpp>
#include <iostream>
namespace pt = boost::posix_time;
namespace dt = boost::date_time;

int main()
{
    switch (pt::time_duration::resolution()) {
    case dt::time_resolutions::sec:
        std::cout << " second\n";
        break;

    case dt::time_resolutions::tenth:
        std::cout << " tenth\n";
        break;

    case dt::time_resolutions::hundredth:
        std::cout << " hundredth\n";
        break;

    case dt::time_resolutions::milli:
        std::cout << " milli\n";
        break;

    case dt::time_resolutions::ten_thousandth:
        std::cout << " ten_thousandth\n";
        break;

    case dt::time_resolutions::micro:
        std::cout << " micro\n";
        break;

    case dt::time_resolutions::nano:
        std::cout << " nano\n";
        break;

    default:
        std::cout << " unknown\n";
        break;
    }

    std::cout << pt::time_duration::num_fractional_digits()
              << '\n';
    std::cout << pt::time_duration::ticks_per_second()
              << '\n';
}

