// Program 4.4 Summing integers - compact version
#include <stdio.h>

int main(void)
{
    unsigned  long long sum = 0LL;            // Stores the sum of the integers
    unsigned int count = 0;                   // The number of integers to be summed
    // Read the number of integers to be summed
    printf("\nEnter the number of integers you want to sum: ");
    scanf(" %u", &count);

    // Sum integers from 1 to count
    for (unsigned int i = 1 ; i <= count ; sum += i++);

    printf("\nTotal of the first %u numbers is %llu\n", count, sum);
    return 0;
}
