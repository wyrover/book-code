//
//  Date.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Date__
#define __FinancialSamples__Date__

#include <string>

class Date {
public:

    enum DayOfWeek {
        Sun,
        Mon,
        Tue,
        Wed,
        Thu,
        Fri,
        Sat 
    };

    Date(int year, int month, int day);
    ~Date();

    bool isLeapYear();
    Date &operator++();
    bool operator<(const Date &d);
    DayOfWeek getDayOfWeek();
    int daysInterval(const Date &);
    bool isTradingDay();
    std::string toStringDate(Date::DayOfWeek day);
private:
    int m_year;
    int m_month;
    int m_day;
};


#endif /* defined(__FinancialSamples__Date__) */
