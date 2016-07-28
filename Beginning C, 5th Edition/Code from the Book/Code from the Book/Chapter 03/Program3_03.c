// Program 3.3 Using nested ifs to analyze numbers
#include <stdio.h>
#include <limits.h>               // For LONG_MAX

int main(void)
{
    long test = 0L;                 // Stores the integer to be checked
    printf("Enter an integer less than %ld:", LONG_MAX);
    scanf(" %ld", &test);

    // Test for odd or even by checking the remainder after dividing by 2
    if (test % 2L == 0L) {
        printf("The number %ld is even", test);

        // Now check whether half the number is also even
        if ((test / 2L) % 2L == 0L) {
            printf("\nHalf of %ld is also even", test);
            printf("\nThat's interesting isn't it?\n");
        }
    } else
        printf("The number %ld is odd\n", test);

    return 0;
}
