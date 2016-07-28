#include <boost/date_time.hpp>
#include <iostream>

namespace greg = boost::gregorian;
namespace pt = boost::posix_time;

int main()
{
    pt::ptime now = pt::second_clock::local_time();
    pt::ptime start_of_day(greg::day_clock::local_day());

    for (pt::time_iterator iter(start_of_day,
                                pt::hours(1)); iter < now; ++iter) {
        std::cout << *iter << '\n';
    }
}

