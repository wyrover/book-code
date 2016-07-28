// Program 3.4A Converting uppercase to lowercase using wide characters
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t letter = 0;                       // Stores a character
    fwprintf(stdout, L"Enter an uppercase letter:");     // Prompt for input
    fwscanf(stdin, L"%lc", &letter);                    // Read a character

    // Check whether the input is uppercase
    if (letter >= L'A') {                     // Is it A or greater?
        if (letter <= L'Z') {                   // and is it Z or lower?
            // It is uppercase
            letter = letter - L'A' + L'a';        // Convert from upper- to lowercase
            fwprintf(stdout, L"You entered an uppercase %lc\n", letter);
        } else                                  // It is not an uppercase letter
            fwprintf(stdout, L"Try using the shift key! I want a capital letter.\n");
    } else
        fwprintf(stdout, L"You did not enter a letter.\n");

    return 0;
}
