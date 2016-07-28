// Program 7.11  A dynamic prime example
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
    unsigned long long *pPrimes = NULL;  // Pointer to primes storage area
    unsigned long long trial = 0;        // Integer to be tested
    bool found = false;                  // Indicates when we find a prime
    int total = 0;                       // Number of primes required
    int count = 0;                       // Number of primes found
    printf("How many primes would you like - you'll get at least 4?  ");
    scanf_s("%d", &total);               // Total is how many we need to find
    total = total < 4 ? 4 : total;       // Make sure it is at least 4
    // Allocate sufficient memory to store the number of primes required
    pPrimes = (unsigned long long*)malloc(total * sizeof(unsigned long long));

    if (!pPrimes) {
        printf("Not enough memory. It's the end I'm afraid.\n");
        return 1;
    }

    // We know the first three primes so let's give the program a start
    *pPrimes = 2ULL;                     // First prime
    *(pPrimes + 1) = 3ULL;               // Second prime
    *(pPrimes + 2) = 5ULL;               // Third prime
    count = 3;                           // Number of primes stored
    trial = 5ULL;                        // Set to the last prime we have

    // Find all the primes required
    while (count < total) {
        trial += 2ULL;                     // Next value for checking

        // Divide by the primes we have. If any divide exactly - it's not prime
        for (int i = 0 ; i < count ; ++i) {
            if (!(found = (trial % * (pPrimes + i))))
                break;                        // Exit if zero remainder
        }

        if (found)                       // We got one - if found is true
            *(pPrimes + count++) = trial;  // Store it and increment count
    }

    // Display primes 5-up
    for (int i = 0 ; i < total ; ++i) {
        printf("%12llu", *(pPrimes + i));

        if (!((i + 1) % 5))
            printf("\n");                   // Newline after every 5
    }

    printf("\n");                       // Newline for any stragglers
    free(pPrimes);                      // Release the heap memory ...
    pPrimes = NULL;                     // ... and reset the pointer
    return 0;
}
