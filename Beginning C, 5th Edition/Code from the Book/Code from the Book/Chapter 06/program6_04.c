// Program 6.4 Joining strings
#define __STDC_WANT_LIB_EXT1__ 1                 // Make optional versions of functions available
#include <string.h>                              // Header for string functions
#include <stdio.h>

int main(void)
{
    char preamble[] = "The joke is:\n\n";
    char str[][40] = {
        "My dog hasn\'t got any nose.\n",
        "How does your dog smell then?\n",
        "My dog smells horrible.\n"
    };
    unsigned int strCount = sizeof(str) / sizeof(str[0]);
    // Find the total length of all the strings in str
    unsigned int length = 0;

    for (unsigned int i = 0 ; i < strCount ; ++i)
        length += strnlen_s(str[i], sizeof(str[i]));

    // Create array to hold all strings combined
    char joke[length + strnlen_s(preamble, sizeof(preamble)) + 1];

    if (strncpy_s(joke, sizeof(joke), preamble, sizeof(preamble))) {
        printf("Error copying preamble to joke.\n");
        return 1;
    }

    // Concatenate strings in joke
    for (unsigned int i = 0 ; i < strCount ; ++i) {
        if (strncat_s(joke, sizeof(joke), str[i], sizeof(str[i]))) {
            printf("Error copying string str[%u].", i);
            return 2;
        }
    }

    printf("%s", joke);
    return 0;
}

