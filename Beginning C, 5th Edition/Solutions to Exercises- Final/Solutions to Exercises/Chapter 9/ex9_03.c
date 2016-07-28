// Exercise 9.3 Join array of strings into a single string
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRINGS 100                                                   // Maximum string count
#define BUFFER_SIZE 50                                                    // Initial input buffer size

char* join_strings(char *strings[], size_t max_length, size_t count);     // Joins array of strings into a single string
char* read_string(char terminator, size_t *length);                       // Reads a string from the keyboard

int main(void)
{
    char *pStrings[MAX_STRINGS];                                            // Array of pointers to strings
    char *joined_strings = NULL;                                            // Pointer to the joined string
    size_t length = 0;                                                      // Length of a string
    size_t max_length = 0;                                                  // Maximum string length
    size_t count = 0;                                                       // Number of strings entered
    char answer = 'y';                                                      // Confirms more input
    char terminator = '*';                                                  // Terminator for string entry

    // Read the strings
    while (count < MAX_STRINGS && tolower(answer) == 'y') {
        printf("Enter a string terminated by an asterisk:\n");

        if (!(pStrings[count++] = read_string(terminator, &length)))
            exit(1);

        if (max_length < length)
            max_length = length;

        printf("Do you want to enter another: ");
        scanf_s(" %c", &answer, sizeof(answer));
        fflush(stdin);                                                        // Lose newline following answer entry
    }

    joined_strings = join_strings(pStrings, max_length, count);
    printf("\nHere are the strings as a single string:\n%s\n", joined_strings);
    free(joined_strings);                                                   // Free memory for joined strings

    for (size_t i = 0 ; i < count ; ++i)                                    // Free memory for original strings
        free(pStrings[i]);

    return 0;
}

/*******************************************************************
 * Function to join an array of strings                            *
 * this function allocates memory that must be freed by the caller *
 * max_length is the length of the longest string, which provides  *
 *  for a safer operation by limiting potential overrun.           *
 *******************************************************************/
char* join_strings(char *strings[], size_t max_length, size_t count)
{
    char* str = NULL;                                                       // Pointer to the joined strings
    size_t total_length = 0;                                                // Total length of joined strings
    size_t length = 0;                                                      // Length of a string

    // Find total length of joined strings
    for (size_t i = 0 ; i < count ; ++i) {
        length = strnlen_s(strings[i], max_length);
        total_length += length;

        if (strings[i][length - 1] != '\n')
            ++total_length;                                                     // For newline to be added as separator
    }

    ++total_length;                                                         // For joined string terminator
    printf("Str length: %zd\n", total_length);
    str = (char*)malloc(total_length);                                      // Allocate memory for joined strings
    str[0] = '\0';                                                          // Empty string we can append to

    // Append all the strings
    for (size_t i = 0 ; i < count ; ++i) {
        strcat_s(str, total_length, strings[i]);
        length = strnlen_s(str, total_length);

        if (str[length - 1] != '\n') {                                        // If last character is not a newline
            str[length] = '\n';                                                 // ...replace terminator by a newline...
            str[length + 1] = '\0';                                             // ...followed by a new terminator
        }
    }

    return str;
}

/***************************************************************************
 * Reads a string of any length.                                           *
 * The string is terminated by the character passed as the argument.       *
 * Memory is allocated to hold the string and must be freed by the caller. *
 * The size of memory occupied by the string is stored in len.             *
 ***************************************************************************/
char* read_string(char terminator, size_t *len)
{
    char *buffer = NULL;                                                    // Pointer to the input buffer
    size_t buffersize = 0;                                                  // Current buffer capacity
    size_t length = 0;                                                      // String length
    char *temp = NULL;                                                      // Temporary buffer pointer
    buffer = (char*)malloc(BUFFER_SIZE);                                    // Initial buffer

    // Read the string character by character
    for (;;) {
        // Check for buffer overflow
        if (length == buffersize) {
            buffersize += BUFFER_SIZE;                                           // Increase buffer size

            if (!(temp = (char*)realloc(buffer, buffersize))) {                  // Allocate new buffer
                printf("Input buffer memory allocation failed.\n");
                return NULL;
            }

            buffer = temp;                                                       // Store new buffer address
        }

        // Check for string terminator
        if ((buffer[length] = getchar()) == terminator)
            break;
        else
            ++length;
    }

    buffer[length] = '\0';                                                  // Append string terminator
    *len = ++length;

    if (!(temp = (char*)realloc(buffer, length))) {                         // Allocate new buffer of exact size
        printf("Input buffer memory allocation failed.\n");
        return NULL;
    }

    return temp;
}
