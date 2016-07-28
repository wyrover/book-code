#include <boost/date_time.hpp>
#include <iostream>
namespace greg = boost::gregorian;

int main()
{
    greg::date d1(1948, greg::Jan, 30);
    greg::date d2(1968, greg::Apr, 4);
    greg::date_duration day_diff = d2 - d1;
    std::cout << day_diff.days()
              << " days between the two dates\n";
    greg::date six_weeks_post_d1 = d1 + greg::weeks(6);
    std::cout << six_weeks_post_d1 << '\n';
    greg::date day_before_d2 = d2 - greg::days(1);
    std::cout << day_before_d2 << '\n';
}

