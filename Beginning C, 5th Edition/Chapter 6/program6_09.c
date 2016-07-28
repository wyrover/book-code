// Program 6.9 Finding occurrences of one string in another
#define __STDC_WANT_LIB_EXT1__ 1       // Make optional versions of functions available
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_LEN 100                   // Maximum input text length
#define SUBSTR_LEN 40                  // Maximum substring length

int main(void)
{
    char text[TEXT_LEN];                 // Input buffer for string to be searched
    char substring[SUBSTR_LEN];          // Input buffer for string sought
    printf("Enter the string to be searched (less than %d characters):\n", TEXT_LEN);
    fgets(text, TEXT_LEN, stdin);
    printf("\nEnter the string sought (less than %d characters):\n", SUBSTR_LEN);
    fgets(substring, SUBSTR_LEN, stdin);
    printf("\nFirst string entered:\n%s\n", text);
    printf("Second string entered:\n%s\n", substring);
    // Overwrite the newline character in each string
    text[strnlen_s(text, TEXT_LEN) - 1] = '\0';
    substring[strnlen_s(substring, SUBSTR_LEN) - 1] = '\0';

    // Convert both strings to uppercase.
    for (int i = 0 ; (text[i] = toupper(text[i])) != '\0' ; ++i);

    for (int i = 0 ; (substring[i] = toupper(substring[i])) != '\0' ; ++i);

    printf("The second string %s found in the first.\n",
           ((strstr(text, substring) == NULL) ? "was not" : "was"));
    return 0;
}
