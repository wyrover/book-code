#include <boost/date_time.hpp>
#include <iostream>
#include <cassert>
#include <ctime>
namespace greg = boost::gregorian;
namespace pt = boost::posix_time;

int main()
{
    pt::ptime pt; // default constructed, is not a time
    assert(pt.is_not_a_date_time());
    // Get current time
    pt::ptime now1 = pt::second_clock::universal_time();
    pt::ptime now2 = pt::from_time_t(std::time(0));
    // Construct from strings
    // Create time points using durations
    pt::ptime pt1(greg::day_clock::universal_day(),
                  pt::hours(10) + pt::minutes(42)
                  + pt::seconds(20) + pt::microseconds(30));
    std::cout << pt1 << '\n';
    // Compute durations
    pt::time_duration dur = now1 - pt1;
    std::cout << dur << '\n';
    std::cout << dur.total_microseconds() << '\n';
    pt::ptime pt2(greg::day_clock::universal_day()),
    pt3 = pt::time_from_string("2015-01-28 10:00:31.83"),
    pt4 = pt::from_iso_string("20150128T151200");
    std::cout << pt2 << '\n' << to_iso_string(pt3) << '\n'
              << to_simple_string(pt4) << '\n';
}

