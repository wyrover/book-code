// Exercise 7.2 Storing and displaying proverbs in order of length
/***************************************************************************
 * This program will read any number of proverbs of any length.            *
 * The input buffer has a default size that is increased automatically     *
 * if it is not large enough.                                              *
 * The same applies to the number of proverbs. If the initial capacity     *
 * for pointers to proverbs is exceeded, a larger space is allocated.      *
 * Additional space is allocated fro recording the lengths of the proverbs.*
 * You could add printf() statements to record when new memory is allocated*
 * Values for BUFFER_LEN, BUFFER_LEN_INCR, and CAPACITY_INCR are set low   *
 * so as to cause frequent reallocation of memory for you to track.        *
 * In a practical program they would be set much higher to avoid           *
 * frequent reallocation of heap memory.                                   *
****************************************************************************/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 5                                                      // Initial length of input buffer
#define BUFFER_LEN_INCR 3                                                 // Increment to buffer length
#define CAPACITY_INCR 2                                                   // Increment to capacity for proverbs

int main(void)
{
    char **pProverbs = NULL;                                               // Pointer to proverb string pointers
    char **temp = NULL;                                                    // Temporary pointer to proverb string pointers
    size_t *pLengths = NULL;                                               // Pointer to proverb lengths
    size_t *ptemplengths = NULL;                                           // Temporary pointer to lengths memory
    int capacity = 0;                                                      // Number of proverbs that can be stored
    int count = 0;                                                         // Number of proverbs read
    char *pstart = NULL;                                                   // Pointer to input buffer start
    char *pstr = NULL;                                                     // Pointer to a string
    int buffer_length = BUFFER_LEN;                                        // Input buffer length
    char *pbuffer = (char*)malloc(BUFFER_LEN);                             // Initial buffer allocation

    if (!pbuffer) {                                                        // If memory was not allocated...
        // ...output a message  and end
        printf("Memory allocation for buffer failed. Terminating program.\n");
        exit(1);
    }

    pstart = pbuffer;                                                      // Store start of buffer

    for (;;) {
        if (count == capacity) {                                             // Do we have space for proverbs?
            // No, so increase the capacity
            capacity += CAPACITY_INCR;
            // Allocate memory for proverb pointers
            temp = realloc(pProverbs, capacity * sizeof(char*));               // Space for pointers to proverbs

            if (!temp) {                                                       // If memory was not allocated
                // Output a message  and end
                printf("Memory allocation failed. Terminating program.\n");
                exit(1);
            }

            pProverbs = temp;                                                  // Copy address of new memory
            temp = NULL;                                                       // Reset pointer
            // Allocate memory for lengths of proverbs
            ptemplengths = realloc(pLengths, capacity * sizeof(size_t));        // Space for lengths proverbs

            if (!ptemplengths) {                                               // If memory was not allocated
                // Output a message  and end
                printf("Memory allocation for proverb lengths failed. Terminating program.\n");
                exit(1);
            }

            pLengths = ptemplengths;                                           // Copy address of new memory
            ptemplengths = NULL;                                               // Reset pointer
        }

        printf("Enter a proverb and press Enter, or just press Enter to end:\n");

        // Read a proverb
        while ((*pbuffer++ = getchar()) != '\n') {
            if (pbuffer - pstart == buffer_length) {                           // Check for buffer full
                buffer_length += BUFFER_LEN_INCR;                                // Increase buffer length
                pstr = realloc(pstart, buffer_length);                           // Allocate new buffer

                if (!pstr) {                                                     // If memory was not allocated
                    // Output a message  and end
                    printf("Memory allocation for input buffer failed. Terminating program.\n");
                    exit(1);
                }

                // The new memory contains the current proverb text but can be at a different address from pstart.
                // We must reset pbuffer to point to the same relative position in the new memory
                // as it did in the old memory and pstart should point to the new buffer.
                pbuffer = pstr + (pbuffer - pstart);                             // Address of next position in new memory
                pstart = pstr;                                                   // Set to start of new buffer
                pstr = NULL;                                                     // Reset pointer
            }
        }

        // check for empty line indicating end of input
        if ((pbuffer - pstart) < 2)
            break;

        *(pbuffer - 1) = '\0';                                               // Add string terminator overwriting newline character
        pbuffer = pstart;
        *(pLengths + count) = strnlen_s(pbuffer, buffer_length);             // Store the proverb length

        if (!(*(pProverbs + count) = (char*)malloc(*(pLengths + count) + 1))) { // Allocate space for the proverb
            // Failed - so output a message  and end
            printf("Memory allocation for input buffer failed. Terminating program.\n");
            exit(1);
        }

        strcpy_s(*(pProverbs + count), *(pLengths + count) + 1,  pbuffer);   // Copy the proverb
        ++count;
    }

    // Order the proverbs from shortest to longest
    size_t length = 0;

    for (size_t i = 0 ; i < count - 2 ; ++i) {
        for (size_t j = i + 1 ; j < count - 1 ; ++j) {
            if (*(pLengths + i) > *(pLengths + j)) {
                // Swap the proverb pointers
                pstr = *(pProverbs + i);
                *(pProverbs + i) = *(pProverbs + j);
                *(pProverbs + j) = pstr;
                // Swap corresponding lengths
                length = *(pLengths + i);
                *(pLengths + i) = *(pLengths + j);
                *(pLengths + j) = length;
            }
        }
    }

    // Output all the strings
    printf("\nIn ascending length order, the proverbs you entered are:\n");

    for (size_t i = 0 ; i < count ; ++i) {
        printf("%s\n", *(pProverbs + i));
        free(*(pProverbs + i));                                             // Release the memory for the proverb
        *(pProverbs + i) = NULL;                                            // Set pointer back to NULL for safety
    }

    free(pProverbs);                                                       // Release memory for the pointers
    free(pstart);                                                          // Release memory for the buffer
    free(pLengths);                                                        // Release memory for the lengths
}

