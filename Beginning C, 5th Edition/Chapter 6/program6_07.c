// Program 6.7 Find all the words
#define __STDC_WANT_LIB_EXT1__ 1                           // Make optional versions of functions available
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(void)
{
    char delimiters[] = " \".,;:!?)(";                       // Prose delimiters
    char buf[100];                                           // Buffer for a line of keyboard input
    char str[1000];                                          // Stores the prose to be tokenized
    char* ptr = NULL;                                        // Pointer used by strtok_s()
    str[0] = '\0';                                           // Set 1st character to null
    size_t str_len = sizeof(str);
    size_t buf_len = sizeof(buf);
    printf("Enter some prose that is less than %d characters.\n"
           "Terminate input by entering an empty line:\n", str_len);

    // Read multiple lines of prose from the keyboard
    while (true) {
        gets_s(buf, buf_len);                                  // Read a line of input

        if (!strnlen_s(buf, buf_len))                          // Empty line ends input
            break;

        if (strcat_s(str, str_len, buf)) {                     // Concatenate the line with str
            printf("Maximum permitted input length exceeded.");
            return 1;
        }
    }

    printf("The words in the prose that you entered are:\n", str);
    // Find and list all the words in the prose
    unsigned int word_count = 0;
    char * pWord = strtok_s(str, &str_len, delimiters, &ptr);  // Find 1st word

    if (pWord) {
        do {
            printf("%-18s", pWord);

            if (++word_count % 5 == 0)
                printf("\n");

            pWord = strtok_s(NULL, &str_len, delimiters, &ptr);    // Find subsequent words
        } while (pWord);                                          // NULL ends tokenizing

        printf("\n%u words found.\n", word_count);
    } else
        printf("No words found.\n");

    return 0;
}
