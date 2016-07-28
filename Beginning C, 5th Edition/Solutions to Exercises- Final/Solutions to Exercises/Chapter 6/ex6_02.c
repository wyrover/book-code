// Exercise 6.2 Analyze comma-separated list of words
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN  5000                                      // Maximum string length
int main(void)
{
    char list[MAX_LEN];                                      // Stores the list of comma separated words
    const char comma[] = ",";                                // The only word delimiter
    printf("Enter a comma separated list of words:\n");
    gets_s(list, sizeof(list));                              // Read the list of words
    // Remove spaces
    size_t index = 0;                                        // Character position
    size_t i = 0;

    do {
        if (isspace(list[i]))                                  // If it's whitespace...
            continue;                                            // ... skip the character...

        list[index++] = list[i];                               // ... otherwise copy the character
    } while (list[i++] != '\0');

    // Find words in list
    char *ptr = NULL;
    size_t list_len = strnlen_s(list, MAX_LEN);
    char *pWord = strtok_s(list, &list_len, comma, &ptr);    // Find 1st word

    if (pWord) {
        do {
            printf("%s\n", pWord);
            pWord = strtok_s(NULL, &list_len, comma, &ptr);      // Find subsequent words
        } while (pWord);                                        // NULL ends tokenizing
    }

    return 0;
}

