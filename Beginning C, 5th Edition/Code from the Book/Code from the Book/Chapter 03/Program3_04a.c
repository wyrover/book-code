// Program 3.4A Converting uppercase to lowercase using wide characters
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    char letter = 0;                          // Stores a character
    printf("Enter an uppercase letter:");     // Prompt for input
    scanf("%c", &letter);                     // Read a character

    // Check whether the input is uppercase
    if (letter >= 'A') {                      // Is it A or greater?
        if (letter <= 'Z') {                    // and is it Z or lower?
            // It is uppercase
            letter = letter - 'A' + 'a';          // Convert from upper- to lowercase
            printf("You entered an uppercase %c\n", letter);
        } else {                                // It is not an uppercase letter
            if (letter >= 'a') {
                if (letter <= 'z')
                    printf("Try using the shift key! I want a capital letter.\n");
            } else
                printf("You didn't enter a letter\n");
        }
    } else
        printf("You didn't enter a letter\n");

    return 0;
}
