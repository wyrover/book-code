// Program 13.2 Demonstrating assertions
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <assert.h>

int main(void)
{
    int y = 5;

    for (int x = 0 ; x < 20 ; ++x) {
        printf("x = %d   y = %d\n", x, y);
        assert(x < y);
    }

    return 0;
}

