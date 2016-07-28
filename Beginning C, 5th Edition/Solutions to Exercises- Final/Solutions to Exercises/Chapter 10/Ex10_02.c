// Exercise 10.2 Reading monetary amounts separated by commas and spaces.
/*
  You only need to ensure that the input format string specifies that '$',
  spaces, and commas are ignored.
*/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#define COUNT 4

int main(void)
{
    double amounts[COUNT] = {0.0};
    double total = 0.0;
    printf_s("Enter the %d amounts:\n", COUNT);

    for (size_t i = 0 ; i < COUNT ; ++i) {
        scanf_s("%*[ ,$]%lf", &amounts[i]);
        total += amounts[i];
    }

    printf_s("The total of the input is: $%.2lf\n", total);
}

