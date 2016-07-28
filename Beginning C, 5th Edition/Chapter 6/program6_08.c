// Program 6.8 Testing characters in a string
#define __STDC_WANT_LIB_EXT1__ 1   // Make optional versions of functions available
#include <stdio.h>
#include <ctype.h>
#define BUF_SIZE 100

int main(void)
{
    char buf[ BUF_SIZE];           // Input buffer
    int nLetters = 0;              // Number of letters in input
    int nDigits = 0;               // Number of digits in input
    int nPunct = 0;                // Number of punctuation characters
    printf("\nEnter an interesting string of less than %d characters:\n", BUF_SIZE);
    gets_s(buf, sizeof(buf));      // Read a string into buffer
    size_t i = 0;                  // Buffer index

    while (buf[i]) {
        if (isalpha(buf[i]))
            ++nLetters;                // Increment letter count
        else if (isdigit(buf[i]))
            ++nDigits;                 // Increment digit count
        else if (ispunct(buf[i]))
            ++nPunct;

        ++i;
    }

    printf("\nYour string contained %d letters, %d digits and %d punctuation characters.\n",
           nLetters, nDigits, nPunct);
    return 0;
}
