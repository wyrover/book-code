// Program 7.12  Extending dynamically allocated memory
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CAP_INCR  10                   // New memory increment

int main(void)
{
    unsigned long long *pPrimes = NULL;  // Pointer to primes storage area
    bool found = false;                  // Indicates when we find a prime
    unsigned long long limit = 0LL;      // Upper limit for primes
    int count = 0;                       // Number of primes found
    printf("Enter the upper limit for primes you want to find: ");
    scanf_s("%llu", &limit);
    // Allocate some initial memory to store primes
    size_t capacity = 10;
    pPrimes = calloc(capacity, sizeof(unsigned long long));

    if (!pPrimes) {
        printf("Not enough memory. It's the end I'm afraid.\n");
        return 1;
    }

    // We know the first three primes so let's give the program a start
    *pPrimes = 2ULL;                     // First prime
    *(pPrimes + 1) = 3ULL;               // Second prime
    *(pPrimes + 2) = 5ULL;               // Third prime
    count = 3;                           // Number of primes stored
    // Find all the primes required starting with the next candidate
    unsigned long long trial = *(pPrimes + 2) + 2ULL;
    unsigned long long *pTemp = NULL;    // Temporary pointer store

    while (trial <= limit) {
        // Divide by the primes we have. If any divide exactly - it's not prime
        for (int i = 1 ; i < count ; ++i)
            if (!(found = (trial % * (pPrimes + i))))
                break;                        // Exit if zero remainder

        if (found) {                     // We got one - if found is true
            if (count == capacity) {
                // We need more memory
                capacity += CAP_INCR;
                pTemp = realloc(pPrimes, capacity * sizeof(unsigned long long));

                if (!pTemp) {
                    printf("Unfortunately memory reallocation failed.\n");
                    free(pPrimes);
                    return 2;
                }

                pPrimes = pTemp;
            }

            *(pPrimes + count++) = trial;  // Store the new prime & increment count
        }

        trial += 2ULL;
    }

    // Display primes 5-up
    printf("%d primes found up to %llu:\n", count, limit);

    for (int i = 0 ; i < count ; ++i) {
        printf("%12llu", *(pPrimes + i));

        if (!((i + 1) % 5))
            printf("\n");                   // Newline after every 5
    }

    printf("\n");                       // Newline for any stragglers
    free(pPrimes);                      // Release the heap memory ...
    pPrimes = NULL;                     // ... and reset the pointer
    return 0;
}
