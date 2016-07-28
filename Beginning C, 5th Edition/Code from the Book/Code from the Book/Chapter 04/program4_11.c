// Program 4.11 Sums of integers with a while loop nested in a for loop
#include <stdio.h>

int main(void)
{
    unsigned long sum = 1UL;         // Stores the sum of integers
    unsigned int j = 1U;             // Inner loop control variable
    unsigned int count = 0;          // Number of sums to be calculated
    // Prompt for, and read the input count
    printf("\nEnter the number of integers you want to sum: ");
    scanf(" %u", &count);

    for (unsigned int i = 1 ; i <= count ; ++i) {
        sum = 1UL;                    // Initialize sum for the inner loop
        j = 1;                        // Initialize integer to be added
        printf("\n1");

        // Calculate sum of integers from 1 to i
        while (j < i) {
            sum += ++j;
            printf(" + %u", j);         // Output +j – on the same line
        }

        printf(" = %lu", sum);        // Output  = sum
    }

    printf("\n");
    return 0;
}
