// Program 9.6 Calculating factorials using recursion
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

unsigned long long factorial(unsigned long long);

int main(void)
{
    unsigned long long number = 0LL;
    printf("Enter an integer value: ");
    scanf_s("%llu", &number);
    printf("The factorial of %llu is %llu\n", number, factorial(number));
    return 0;
}

// A recursive factorial function
unsigned long long factorial(unsigned long long n)
{
    if (n < 2LL)
        return n;

    return n * factorial(n - 1LL);
}
