// Program 10.11 Outputting character data
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <limits.h>               // For CHAR_MAX
#include <ctype.h>                // For isprint()

int main(void)
{
    int count = 0;
    printf_s("The printable characters are the following:\n");

    // Iterate over all values of type char
    for (int code = 0 ; code <= CHAR_MAX ; ++code) {
        char ch = (char)code;

        if (isprint(ch)) {
            if (count++ % 32 == 0)
                printf_s("\n");

            printf_s(" %c", ch);
        }
    }

    printf_s("\n");
    return 0;
}

