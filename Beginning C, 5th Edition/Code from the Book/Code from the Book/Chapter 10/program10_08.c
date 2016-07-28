// Program 10.8 Integer output variations
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    int        i = 15,       j = 345,            k = 4567;
    long long li = 56789LL, lj = 67891234567LL, lk = 23456789LL;
    printf_s("i = %d   j = %d    k = %d   i = %6.3d   j = %6.3d   k = %6.3d\n", i , j, k, i, j, k);
    printf_s("i = %-d   j = %+d   k = %-d   i = %-6.3d   j = %-6.3d   k = %-6.3d\n", i , j, k, i, j, k);
    printf_s("li = %d   lj = %d   lk = %d\n", li, lj, lk);
    printf_s("li = %lld   lj = %lld   lk = %lld\n", li, lj, lk);
    return 0;
}

