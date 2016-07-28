// Program 13.4        Getting date data with ease
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <time.h>

int main(void)
{
    const char *day[7] = {
        "Sunday"  , "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };
    const char *month[12] = {
        "January",   "February", "March",    "April",
        "May",       "June",     "July",     "August",
        "September", "October",  "November", "December"
    };
    const char *suffix[] = { "st", "nd", "rd", "th" };
    enum sufindex { st, nd, rd, th } sufsel = th;       // Suffix selector
    struct tm ourT;                                     // The time structure
    time_t tVal = time(NULL);                           // Calendar time

    if (!localtime_s(&tVal, &ourT)) {                   // Populate time structure
        fprintf_s(stderr, "Failed to populate tm struct.\n");
        return -1;
    }

    switch (ourT.tm_mday) {
    case 1:
    case 21:
    case 31:
        sufsel = st;
        break;

    case 2:
    case 22:
        sufsel = nd;
        break;

    case 3:
    case 23:
        sufsel = rd;
        break;

    default:
        sufsel = th;
        break;
    }

    printf_s("Today is %s the %d%s %s %d. ", day[ourT.tm_wday],
             ourT.tm_mday, suffix[sufsel], month[ourT.tm_mon], 1900 + ourT.tm_year);
    printf_s("The time is %d : %d : %d.\n",
             ourT.tm_hour, ourT.tm_min, ourT.tm_sec);
    return 0;
}

