// Program 7.14 Using array notation with pointers to sort strings
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUF_LEN 100                    // Length of input buffer
#define COUNT        5                 // Initial number of strings

int main(void)
{
    char buf[BUF_LEN];                  // Input buffer
    size_t str_count = 0;               // Current string count
    size_t capacity = COUNT;            // Current maximum number of strings
    char **pS = calloc(capacity, sizeof(char*));    // Pointers to strings
    char** psTemp = NULL;               // Temporary pointer to pointer to char
    char* pTemp = NULL;                 // Temporary pointer to char
    size_t str_len = 0;                 // Length of a string
    bool sorted = false;                // Indicated when strings are sorted
    printf("Enter strings to be sorted, one per line. Press Enter to end:\n");
    // Read in all the strings
    char *ptr = NULL;

    while (true) {
        ptr = fgets(buf, BUF_LEN, stdin);

        if (!ptr) {                       // Check for read error
            printf("Error reading string.\n");
            free(pS);
            pS = NULL;
            return 1;
        }

        if (*ptr == '\n') break;          // Empty line check

        if (str_count == capacity) {
            capacity += capacity / 4;        // Increase capacity by 25%

            if (!(psTemp = realloc(pS, capacity))) return 1;

            pS = psTemp;
        }

        str_len = strnlen_s(buf, BUF_LEN) + 1;

        if (!(pS[str_count] = malloc(str_len))) return 2;

        strcpy_s(pS[str_count++], str_len, buf);
    }

    // Sort the strings in ascending order
    while (!sorted) {
        sorted = true;

        for (size_t i = 0 ; i < str_count - 1 ; ++i) {
            if (strcmp(pS[i], pS[i + 1]) > 0) {
                sorted = false;               // We were out of order so...
                pTemp = pS[i];                // swap pointers pS[i]...
                pS[i] = pS[i + 1];            //       and...
                pS[i + 1]  = pTemp;           //     pS[i + 1]
            }
        }
    }

    // Output the sorted strings
    printf("Your input sorted in ascending sequence is:\n\n");

    for (size_t i = 0 ; i < str_count ; ++i) {
        printf("%s", pS[i]);
        free(pS[i]);                      // Release memory for the word
        pS[i] = NULL;                     // Reset the pointer
    }

    free(pS);                           // Release the memory for pointers
    pS = NULL;                          // Reset the pointer
    return 0;
}
