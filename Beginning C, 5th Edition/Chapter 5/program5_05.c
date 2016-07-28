// Program 5.5 Using the & operator
#include<stdio.h>

int main(void)
{
    // Define some integer variables
    long a = 1L;
    long b = 2L;
    long c = 3L;
    // Define some floating-point variables
    double d = 4.0;
    double e = 5.0;
    double f = 6.0;
    printf("A variable of type long occupies %u bytes.", sizeof(long));
    printf("\nHere are the addresses of some variables of type long:");
    printf("\nThe address of a is: %p  The address of b is: %p", &a, &b);
    printf("\nThe address of c is: %p", &c);
    printf("\n\nA variable of type double occupies %u bytes.", sizeof(double));
    printf("\nHere are the addresses of some variables of type double:");
    printf("\nThe address of d is: %p  The address of e is: %p", &d, &e);
    printf("\nThe address of f is: %p\n", &f);
    return 0;
}
