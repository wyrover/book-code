// Exercise 13.2 A function to produce a string containing the current time
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

/******************************************
 * Function that returns time as a string *
 * arg = 0 result is in 12 hour format    *
 * arg = 1 result is in 24 hour format    *
 ******************************************/
char *time_str(int arg)
{
    bool afternoon = true;
    time_t now = time(NULL);
    struct tm *ptime = localtime(&now);

    if (!arg) {
        afternoon = ptime->tm_hour > 12;
        ptime->tm_hour %= 12;
    }

    static char result[12];
    sprintf_s(result, sizeof(result), "%2d:%02d:%02d", ptime->tm_hour, ptime->tm_min, ptime->tm_sec);

    if (!arg) {
        strcat_s(result, sizeof(result), afternoon ? " pm" : " am");
    }

    return result;
}

int main(void)
{
    printf_s("Time is %s\n", time_str(0));
    printf_s("Time is %s\n", time_str(1));
    return 0;
}
