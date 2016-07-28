// Exercise 13.1 Defining a COMPARE(x,y) macro
/*
  One advantage of the COMPARE macro is that is works with numeric values of any type.
  Another advantage is that there is no function call overhead.
*/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

#define COMPARE(x,y) (((x)<(y)) ? -1 : (((x)==(y)) ? 0 : 1))

int main(void)
{
    int a = 5, b = 5, c = 10;
    printf_s("COMPARE(%d, %d) = %d\n", a, b, COMPARE(a, b));
    printf_s("COMPARE(%d, %d) = %d\n", a, c, COMPARE(a, c));
    printf_s("COMPARE(%d, %d) = %d\n", c, b, COMPARE(c, b));
    printf_s("COMPARE(%d, %d) = %d\n", a + b, c - b, COMPARE(a + b, c - b));
    double x = 24.5, y = 28.0, z = 3.5;
    printf_s("COMPARE(%.2f, %.2f) = %d\n", x, y, COMPARE(x, y));
    printf_s("COMPARE(%.2f, %.2f) = %d\n", y, z, COMPARE(y, z));
    printf_s("COMPARE(%.2f, %.2f) = %d\n", x + z, y, COMPARE(x + z, y));
    return 0;
}
