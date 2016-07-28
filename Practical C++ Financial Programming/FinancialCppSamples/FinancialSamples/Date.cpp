//
//  Date.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Date.h"

#include <vector>
#include <algorithm>

#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

Date::Date(int year, int month, int day)
: m_year(year),
  m_month(month),
  m_day(day)
{
}

Date::~Date()
{
}

bool Date::isLeapYear()
{
	if (m_year % 4 != 0) return false;
	if (m_year % 100 != 0) return true;
	if (m_year % 400 != 0) return false;
	return true;
}

Date &Date::operator++()
{
	std::vector<int> m31 = { 1, 3, 5, 7, 8, 10, 12 };

	if (m_day == 31)
	{
		m_day = 1;
		m_month++;
	}
	else if (m_day == 30 && std::find(m31.begin(), m31.end(), m_month) == m31.end())
	{
		m_day = 1;
		m_month++;
	}
	else if (m_day == 29 && m_month == 2)
	{
		m_day = 1;
		m_month++;
	}
	else if (m_day == 28 && m_month == 2  && !isLeapYear())
	{
		m_day = 1;
		m_month++;
	}
	else
	{
		m_day++;
	}

	if (m_month > 12)
	{
		m_month = 1;
		m_year++;
	}
	return *this;
}

int Date::daysInterval(const Date &d)
{
    date bdate1(m_year, m_month, m_day);
    date bdate2(d.m_year, d.m_month, d.m_day);

    boost::gregorian::date_duration duration = bdate1 - bdate2;
    return (int) duration.days();
}

bool Date::operator<(const Date &d)
{
    if (m_year < d.m_year) return true;
    if (m_year == d.m_year && m_month < d.m_month) return true;
    if (m_year == d.m_year && m_month == d.m_month && m_day < d.m_day) return true;
    return false;
}

Date::DayOfWeek Date::getDayOfWeek()
{
    int day = 1;
    Date d(1900, 1, 1);
    for (;d < *this; ++d)
    {
        if (day == 7) day = 1;
        else day++;
    }
    return (DayOfWeek) day;
}

bool Date::isTradingDay()
{
    DayOfWeek dayOfWeek = getDayOfWeek();
    if (dayOfWeek == Sun || dayOfWeek == Sat)
    {
        return false;
    }
    return true;
}

std::string Date::toStringDate(Date::DayOfWeek day)
{
    switch(day)
    {
        case DayOfWeek::Sun: return "Sunday";
        case DayOfWeek::Mon: return "Monday";
        case DayOfWeek::Tue: return "Tuesday";
        case DayOfWeek::Wed: return "Wednesday";
        case DayOfWeek::Thu: return "Thursday";
        case DayOfWeek::Fri: return "Friday";
        case DayOfWeek::Sat: return "Saturday";
    }
    throw std::runtime_error("unknown day of week");
}


int main_date()
{
    Date myDate(2015, 1, 3);
    auto dayOfWeek = myDate.getDayOfWeek();
    std::cout << " day of week is "
        << myDate.toStringDate(dayOfWeek) << std::endl;
    Date secondDate(2014, 12, 5);
    ++secondDate;  // test increment operator
    ++secondDate;

    int interval = myDate.daysInterval(secondDate);
    std::cout << " interval is " << interval << " days" << std::endl;
    return 0;
}
