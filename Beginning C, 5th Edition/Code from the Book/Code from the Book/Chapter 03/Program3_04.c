// Program 3.4 Converting uppercase to lowercase
#include <stdio.h>

int main(void)
{
    char letter = 0;                          // Stores a character
    printf("Enter an uppercase letter:");     // Prompt for input
    scanf("%c", &letter);                     // Read a character

    // Check whether the input is uppercase
    if (letter >= 'A')                        // Is it A or greater?
        if (letter <= 'Z') {                    // and is it Z or lower?
            // It is uppercase
            letter = letter - 'A' + 'a';          // Convert from upper- to lowercase
            printf("You entered an uppercase %c\n", letter);
        } else                                  // It is not an uppercase letter
            printf("Try using the shift key! I want a capital letter.\n");

    return 0;
}
