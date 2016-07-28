#include <boost/date_time.hpp>
#include <iostream>
namespace greg = boost::gregorian;
namespace dt = boost::date_time;

int main()
{
    greg::date startCal(2015, greg::Jan, 1);
    greg::date endCal(2015, greg::Dec, 31);
    greg::date startFiscal(2014, greg::Oct, 1);
    greg::date endFiscal(2015, greg::Sep, 30);
    greg::date_period cal(startCal, endCal);
    greg::date_period fisc(startFiscal, endFiscal);
    std::cout << "Fiscal year begins " << fisc.begin()
              << " and ends " << fisc.end() << '\n';

    if (cal.intersects(fisc)) {
        auto overlap = cal.intersection(fisc);
        greg::month_iterator miter(overlap.begin());

        while (*miter < overlap.end()) {
            greg::last_day_of_the_week_in_month
            last_weekday(greg::Friday, miter->month());
            std::cout << last_weekday.get_date(miter->year())
                      << '\n';
            ++miter;
        }
    }
}

