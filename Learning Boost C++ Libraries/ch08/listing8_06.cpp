#include <boost/date_time.hpp>
#include <iostream>
#include <cassert>
namespace greg = boost::gregorian;
namespace pt = boost::posix_time;

int main()
{
    // Get current time
    pt::ptime now1 = pt::second_clock::local_time();
    pt::time_period starts_now(now1, pt::hours(2));
    assert(starts_now.length() == pt::hours(2));
    auto later1 = now1 + pt::hours(1);
    pt::time_period starts_in_1(later1, pt::hours(3));
    assert(starts_in_1.length() == pt::hours(3));
    auto later2 = now1 + pt::hours(3);
    pt::time_period starts_in_3(later2, pt::hours(1));
    assert(starts_in_3.length() == pt::hours(1));
    std::cout << "starts_in_1 starts at " << starts_in_1.begin()
              << " and ends at " << starts_in_1.last() << '\n';
    // comparing time periods
    // non-overlapping
    assert(starts_now < starts_in_3);
    assert(!starts_now.intersects(starts_in_3));
    // overlapping
    assert(starts_now.intersects(starts_in_1));
    assert(starts_in_1.contains(starts_in_3));
}

