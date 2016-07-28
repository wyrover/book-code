// Exercise 10.4 A function to read a string terminated by an arbitrary character.
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>

#define MAX_SIZE     100
#define STRING_COUNT   5

char* getString(char *buffer, size_t buffer_size, char end_char);

int main(void)
{
    char buffer[MAX_SIZE] = {0};
    int i = 0;

    for (i = 0 ; i < STRING_COUNT ; ++i) {
        printf_s("\nEnter a string terminated by a semi-colon:\n");

        if (!getString(buffer, MAX_SIZE,  ':')) {
            printf_s("Error return but continuing...\n");
            continue;
        }

        printf_s("The string you entered was:\n%s\n", buffer);
    }
}

char* getString(char *buffer, size_t size, char end_char)
{
    size_t i = 0;

    // Read a character until end_char is entered
    while ((buffer[i++] = getchar()) != end_char) {
        if (i >= size) {
            printf_s("Buffer capacity exceeded.\n");
            return NULL;
        }
    }

    fflush(stdin);                                 // Clear out newline
    buffer[i - 1] = '\0';                          // Overwrite end_char with string terminator
    return buffer;
}
