#include <boost/date_time.hpp>
#include <iostream>
#include <cassert>
namespace greg = boost::gregorian;

int main()
{
    greg::date d0;  // default constructed, is not a date
    assert(d0.is_not_a_date());
    // Construct dates from parts
    greg::date d1(1948, greg::Jan, 30);
    greg::date d2(1968, greg::Apr, 4);
    // Construct dates from string representations
    greg::date dw1 = greg::from_uk_string("15/10/1948");
    greg::date dw2 = greg::from_simple_string("1956-10-29");
    greg::date dw3 = greg::from_undelimited_string("19670605");
    greg::date dw4 = greg::from_us_string("10-06-1973");
    // Current date
    greg::date today = greg::day_clock::local_day();
    greg::date londonToday = greg::day_clock::universal_day();
    // Take dates apart
    std::cout << today.day_of_week() << " " << today.day() << ", "
              << today.month() << ", " << today.year() << '\n';
}

