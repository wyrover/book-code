#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>
#include <cassert>
namespace qi = boost::spirit::qi;

struct timestamp_t {
    void setYear(short val)
    {
        year = val;
    }
    unsigned short getYear()
    {
        return year;
    }

    void setMonth(short val)
    {
        month = val;
    }
    unsigned short getMonth()
    {
        return month;
    }

    void setDay(short val)
    {
        day = val;
    }
    unsigned short getDay()
    {
        return day;
    }

    void setHours(short val)
    {
        hours = val;
    }
    unsigned short getHours()
    {
        return hours;
    }

    void setMinutes(short val)
    {
        minutes = val;
    }
    unsigned short getMinutes()
    {
        return minutes;
    }

    void setSeconds(short val)
    {
        seconds = val;
    }
    unsigned short getSeconds()
    {
        return seconds;
    }

    void setFractions(short val)
    {
        fractions = val;
    }
    unsigned short getFractions()
    {
        return fractions;
    }

private:
    unsigned short year, month, day,
             hours, minutes, seconds, fractions;
};

timestamp_t parseTimeStamp(std::string input)
{
    timestamp_t ts;
    qi::int_parser<unsigned short, 10, 4, 4> year_p;
    qi::int_parser<unsigned short, 10, 2, 2> month_p, day_p,
    hour_p, min_p, sec_p;
    qi::rule<std::string::iterator> date_p =
        year_p [boost::bind(&timestamp_t::setYear, &ts, ::_1)]
        >> qi::char_('-')
        >> month_p [boost::bind(&timestamp_t::setMonth, &ts, ::_1)]
        >> qi::char_('-')
        >> day_p [boost::bind(&timestamp_t::setDay, &ts, ::_1)];
    qi::rule<std::string::iterator> seconds_p =
        sec_p [boost::bind(&timestamp_t::setSeconds, &ts, ::_1)]
        >> -(qi::char_('.')
             >> qi::ushort_
             [boost::bind(&timestamp_t::setFractions, &ts, ::_1)]);
    qi::rule<std::string::iterator> time_p =
        hour_p  [boost::bind(&timestamp_t::setHours, &ts, ::_1)]
        >> qi::char_(':')
        >> min_p [boost::bind(&timestamp_t::setMinutes, &ts, ::_1)]
        >> -(qi::char_(':') >> seconds_p);
    qi::rule<std::string::iterator> timestamp_p = date_p >> -
            (qi::space >> time_p);
    auto iterator = input.begin();
    bool success = qi::phrase_parse(iterator, input.end(),
                                    timestamp_p, qi::space);
    assert(success);
    return ts;
}

int main()
{
    std::string input = "2015-02-18 10:04:31.29";
    timestamp_t ts = parseTimeStamp(input);
    assert(ts.getYear() == 2015 && ts.getMonth() == 2 &&
           ts.getDay() == 18 && ts.getHours() == 10 &&
           ts.getMinutes() == 4 && ts.getSeconds() == 31 &&
           ts.getFractions() == 29);
}
