// Program 6.5 Comparing strings
#define __STDC_WANT_LIB_EXT1__ 1                           // Make optional versions of functions available#include <stdio.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 21                                      // Maximum char array length
int main(void)
{
    char word1[MAX_LENGTH];                                  // Stores the first word
    char word2[MAX_LENGTH];                                  // Stores the second word
    printf("Type in the first word (maximum %d characters): ", MAX_LENGTH - 1);
    int retval = scanf_s("%s", word1, sizeof(word1));        // Read the first word

    if (EOF == retval) {
        printf("Error reading the word.\n");
        return 1;
    }

    printf("Type in the second word (maximum %d characters): ", MAX_LENGTH - 1);
    retval = scanf_s("%s", word2, sizeof(word2));            // Read the second word

    if (EOF == retval) {
        printf("Error reading the word.\n");
        return 2;
    }

    // Compare the words
    if (strcmp(word1, word2) == 0)
        printf("You have entered identical words");
    else
        printf("%s precedes %s\n",
               (strcmp(word1, word2) < 0) ? word1 : word2,
               (strcmp(word1, word2) < 0) ? word2 : word1);

    return 0;
}
