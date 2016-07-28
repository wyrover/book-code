// Program 11.8 The operation of a union
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

typedef union UDate UDate;
typedef struct Date Date;
typedef struct MixedDate MixedDate;
typedef struct NumericDate NumericDate;

void print_date(const Date* date);            // Prototype

enum Date_Format {numeric, text, mixed};      // Date formats

// Date in the form "day" "date month" nnnn
struct MixedDate {
    char *day;
    char *date;
    int year;
};

// Date in the form dd mm yyyy
struct NumericDate {
    int day;
    int month;
    int year;
};

// Any of 3 possible date forms
union UDate {
    char *date_str;
    MixedDate day_date;
    NumericDate nDate;
};

// A date in any form
struct Date {
    enum Date_Format format;
    UDate date;
};

int main(void)
{
    NumericDate yesterday = { 11, 11, 2012};
    MixedDate today = {"Monday", "12th November", 2012};
    char tomorrow[] = "Tues 13th Nov 2012";
    // Create Date object with a numeric date
    UDate udate = {tomorrow};
    Date the_date;
    the_date.date = udate;
    the_date.format = text;
    print_date(&the_date);
    // Create Date object with a text date
    the_date.date.nDate = yesterday;
    the_date.format = numeric;
    print_date(&the_date);
    // Create Date object with a mixed date
    the_date.date.day_date = today;
    the_date.format = mixed;
    print_date(&the_date);
    return 0;
}

// Outputs a date
void print_date(const Date* date)
{
    switch (date->format) {
    case numeric:
        printf_s("The date is %d/%d/%d.\n", date->date.nDate.day,
                 date->date.nDate.month,
                 date->date.nDate.year);
        break;

    case text:
        printf_s("The date is %s.\n", date->date.date_str);
        break;

    case mixed:
        printf_s("The date is %s %s %d.\n", date->date.day_date.day,
                 date->date.day_date.date,
                 date->date.day_date.year);
        break;

    default:
        printf_s("Invalid date format.\n");
    }
}

