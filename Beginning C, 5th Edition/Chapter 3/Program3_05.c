// Program 3.5   Testing letters an easier way
#include <stdio.h>
int main(void)
{
    char letter = 0;                               // Stores an input character
    printf("Enter an upper case letter:");         // Prompt for input
    scanf(" %c", &letter);                         // Read the input character

    if ((letter >= 'A') && (letter <= 'Z')) {      // Verify uppercase letter
        letter += 'a' - 'A';                         // Convert to lowercase
        printf("You entered an uppercase %c.\n", letter);
    } else
        printf("You did not enter an uppercase letter.\n");

    return 0;
}
