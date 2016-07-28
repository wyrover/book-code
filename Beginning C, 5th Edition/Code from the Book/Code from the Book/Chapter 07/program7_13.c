// Program 7.13 Extending dynamically allocated memory for strings
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUF_LEN      100                                   // Input buffer size
#define INIT_STR_EXT  50                                   // Initial space for prose
#define WORDS_INCR     5                                   // Words capacity increment

int main(void)
{
    char delimiters[] = " \n\".,;:!?)(";                     // Prose delimiters
    char buf[BUF_LEN];                                       // Buffer for a line of keyboard input
    size_t str_size = INIT_STR_EXT;                          // Current memory to store prose
    char* pStr = malloc(str_size);                           // Pointer to prose to be tokenized
    *pStr = '\0';                                            // Set 1st character to null
    printf("Enter some prose with up to %d characters per line.\n"
           "Terminate input by entering an empty line:\n", BUF_LEN);

    // Read multiple lines of prose from the keyboard
    while (true) {
        fgets(buf, BUF_LEN, stdin);                            // Read a line of input

        if (buf[0] == '\n')                                    // An empty line ends input
            break;

        if (strnlen_s(pStr, str_size) + strnlen_s(buf, BUF_LEN) + 1 > str_size) {
            str_size = strnlen_s(pStr, str_size) + strnlen_s(buf, BUF_LEN) + 1;
            pStr = realloc(pStr, str_size);
        }

        if (strcat_s(pStr, str_size, buf)) {                     // Concatenate the line with pStr
            printf("Something's wrong. String concatenation failed.\n");
            return 1;
        }
    }

    // Find and list all the words in the prose
    size_t maxWords = 10;                                     // Current maximum word count
    int word_count = 0;                                       // Current word count
    size_t word_length = 0;                                   // Current word length
    char** pWords = calloc(maxWords, sizeof(char*));          // Stores pointers to the words
    int* pnWord = calloc(maxWords, sizeof(int));              // Stores count for each word
    size_t str_len = strnlen_s(pStr, str_size);                // Length used by strtok_s()
    char* ptr = NULL;                                          // Pointer used by strtok_s()
    char* pWord = strtok_s(pStr, &str_len, delimiters, &ptr);  // Find 1st word

    if (!pWord) {
        printf("No words found. Ending program.\n");
        return 1;
    }

    bool new_word = true;                                     // False for an existing word

    while (pWord) {
        // Check for existing word
        for (int i = 0 ; i < word_count ; ++i) {
            if (strcmp(*(pWords + i), pWord) == 0) {
                ++*(pnWord + i);
                new_word = false;
                break;
            }
        }

        if (new_word) {                                         // Check for new word
            //Check for sufficient memory
            if (word_count == maxWords) {
                // Get more space for pointers to words
                maxWords += WORDS_INCR;
                pWords = realloc(pWords, maxWords * sizeof(char*));
                // Get more space for word counts
                pnWord = realloc(pnWord, maxWords * sizeof(int));
            }

            // Found a new word so get memory for it and copy it there
            word_length = ptr - pWord;      // Length of new word
            *(pWords + word_count) = malloc(word_length);         // Allocate memory for word
            strcpy_s(*(pWords + word_count), word_length, pWord); // Copy to array
            *(pnWord + word_count++) = 1;                         // Set new word count
        } else
            new_word = true;                                      // Reset new word flag

        pWord = strtok_s(NULL, &str_len, delimiters, &ptr);      // Find subsequent word
    }

    // Output the words and counts
    for (int i = 0; i < word_count ; ++i) {
        printf("  %-13s  %3d", *(pWords + i), *(pnWord + i));

        if ((i + 1) % 4 == 0)
            printf("\n");
    }

    printf("\n");

    // Free the memory for words
    for (int i = 0; i < word_count ; ++i) {
        free(*(pWords + i));                                    // Free memory for word
        *(pWords + i) = NULL;                                   // Reset the pointer
    }

    free(pWords);                                             // Free memory for pointers to words
    pWords = NULL;
    free(pnWord);                                             // Free memory for word counts
    pnWord = NULL;
    free(pStr);                                               // Free memory for prose
    pStr = NULL;
    return 0;
}
