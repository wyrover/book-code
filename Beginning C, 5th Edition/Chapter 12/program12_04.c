// Program 12.4  A prime example using binary files
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>                                          // For square root function sqrt()

#define PER_LINE     8                                     // Primes per line in output
#define MEM_PRIMES 10*PER_LINE                             // Lines of primes in memory

// Function prototypes
bool is_prime(unsigned long long N);
void list_primes(void);
int check(unsigned long long buffer[], size_t count, unsigned long long N);
void list_array(void);
void write_file(void);

// Anonymous struct type
struct {
    char *filename;                                               // File name for primes
    FILE *pfile;                                                  // File stream pointer
    unsigned long long primes[MEM_PRIMES];                        // Array to store primes
    size_t count;                                                 // Free location in primes
} global = { .filename = "myfile.bin",                          // Physical file name
             .pfile = NULL,                                     // File pointer value
             .primes = {2ULL, 3ULL, 5ULL},                      // Initial seed primes
             .count = 3                                         // Number of primes in array
           };

int main(void)
{
    unsigned long long trial = global.primes[global.count - 1];   // Current prime candidate
    size_t num_primes = 3UL;                                      // Current prime count
    size_t total = 0UL;                                           // Total number required
    printf_s("How many primes would you like?  ");
    scanf_s("%llu", &total);
    total = total < 4 ? 4 : total;                                // Make sure it is at least 4

    // Prime finding and storing loop
    while (num_primes < total) {                                  // Loop until we get total required
        trial += 2ULL;                                              // Next value for checking

        if (is_prime(trial)) {                                      // Is trial prime?
            global.primes[global.count++] = trial;                    // Yes, so store it
            ++num_primes;                                             // Increment total number of primes

            if (global.count == MEM_PRIMES)                           // If array is full...
                write_file();                                           // ...write to file.
        }
    }

    // If there are primes in the array and there is a file, write them to the file.
    if (global.pfile && global.count > 0)
        write_file();

    list_primes();                                                // Display the file contents

    if (global.pfile)                                             // If we needed a file...
        if (remove(global.filename))                                // ...then delete it.
            printf_s("\nFailed to delete %s\n", global.filename);     // Delete failed
        else
            printf_s("\nFile %s deleted.\n", global.filename);        // Delete OK

    return 0;
}

/*******************************************************************
 * Function to test if a number, N, is prime using primes in       *
 * memory and on file                                              *
 * First parameter n - value to be tested                          *
 * Return value - a positive value for a prime, zero otherwise     *
 *******************************************************************/
bool is_prime(unsigned long long n)
{
    unsigned long long buffer[MEM_PRIMES];                        // local buffer for primes from file
    size_t count = 0;                                             // Number of primes in buffer
    int k = 0;

    if (global.pfile) {                                           // If we have written a file...
        // ...open it
        if (fopen_s(&global.pfile, global.filename, "rb")) {
            printf_s("Unable to open %s to read.\n", global.filename);
            exit(1);
        }

        setbuf(global.pfile, NULL);

        while (!feof(global.pfile)) {
            // Check against primes in the file first
            // Read primes
            count = fread(buffer, sizeof(buffer[0]), MEM_PRIMES, global.pfile);

            if ((k = check(buffer, count, n)) == 1) {                 // Prime?
                fclose(global.pfile);                                   // Yes, so close the file
                return true;                                            // and return
            }
        }

        fclose(global.pfile);                                       // Close the file
    }

    return 1 == check(global.primes, global.count, n);            // Check against primes in memory
}

/*******************************************************************
 * Function to check whether an integer, n, is divisible by any    *
 * of the elements in the array buffer up to the square root of n. *
 * First parameter buffer - an array of primes                     *
 * second parameter count - number of elements in buffer           *
 * Third parameter n - the value to be checked                     *
 * Return value:  +1 if n is prime,                                *
 *                 0 if n is not prime                             *
 *                -1 for more checks needed                        *
 *******************************************************************/
int check(unsigned long long buffer[], size_t count, unsigned long long n)
{
    // Upper limit
    unsigned long long root_N = (unsigned long long)(1.0 + sqrt(n));

    for (size_t i = 0 ; i < count ; ++i) {
        if (n % buffer[i] == 0ULL)                                  // Exact division?
            return 0;                                                 // Then not a prime

        if (buffer[i] > root_N)                                     // Divisor exceeds square root?
            return 1;                                                 // Then must be a prime
    }

    return -1;                                                    // More checks necessary...
}

/*******************************************
 * Function to output all the primes       *
 *******************************************/
void list_primes(void)
{
    if (global.pfile) {
        if (fopen_s(&global.pfile, global.filename, "rb")) {        // Open the file
            printf_s("\nUnable to open %s to read primes for output\n", global.filename);
            exit(1);
        }

        setbuf(global.pfile, NULL);

        while (!feof(global.pfile)) {
            global.count = fread(global.primes, sizeof(unsigned long long), MEM_PRIMES, global.pfile);
            list_array();
        }

        printf_s("\n");
        fclose(global.pfile);
    } else
        list_array();
}

/*******************************************
 * List primes in the global primes array  *
 *******************************************/
void list_array(void)
{
    for (size_t j = 0 ; j < global.count ; ++j) {                 // Display the primes
        printf_s("%10llu", global.primes[j]);                       // Output a prime

        if (((j + 1) % PER_LINE) == 0)
            printf_s("\n");
    }
}

/*******************************************
 * Write primes in memory to the file      *
 *******************************************/
void write_file(void)
{
    if (fopen_s(&global.pfile, global.filename, "ab")) {
        // Failed, so explain and end the program
        printf_s("Unable to open %s to append\n", global.filename);
        exit(1);
    }

    setbuf(global.pfile, NULL);
    // Write the array to file
    fwrite(global.primes, sizeof(unsigned long long), global.count, global.pfile);
    fclose(global.pfile);                                         // Close the file
    global.count = 0;                                             // Reset count of primes in memory
}
