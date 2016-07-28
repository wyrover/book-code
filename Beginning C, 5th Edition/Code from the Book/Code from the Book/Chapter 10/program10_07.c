// Program 10.7 Reading and unreading characters
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH 50                      // Name buffer size

// Function prototypes
void eatspaces(void);
bool getinteger(int *n);
char *getname(char *name, size_t length);
bool isnewline(void);

int main(void)
{
    int number;
    char name[LENGTH] = {'\0'};
    printf("Enter a sequence of integers and alphabetic names in a single line:\n");

    while (!isnewline()) {
        if (getinteger(&number))
            printf("Integer value:%8d\n", number);
        else if (strnlen_s(getname(name, LENGTH), LENGTH) > 0)
            printf("Name: %s\n", name);
        else {
            printf("Invalid input.\n");
            return 1;
        }
    }

    return 0;
}

// Function to ignore spaces from standard input
void eatspaces(void)
{
    char ch = 0;

    while (isspace(ch = (char)getchar()));

    ungetc(ch, stdin);
}

// Function to read an integer from standard input
bool getinteger(int *n)
{
    eatspaces();
    int value = 0;
    int sign = 1;
    char ch = 0;

    // Check first character
    if ((ch = (char)getchar()) == '-')           // should be minus
        sign = -1;
    else if (isdigit(ch))                // ...or a digit
        value = ch - '0';
    else  if (ch != '+') {               // ...or plus
        ungetc(ch, stdin);
        return false;                      // Not an integer
    }

    // Find more digits
    while (isdigit(ch = (char)getchar()))
        value = 10 * value + (ch - '0');

    ungetc(ch, stdin);                   // Push back non-digit character
    *n = value * sign;                   // Set the sign
    return true;
}

// Function to read an alphabetic name from input
char* getname(char *name, size_t length)
{
    eatspaces();                         // Remove leading spaces
    size_t count = 0;
    char ch = 0;

    while (isalpha(ch = (char)getchar())) {    // As long as there are letters...
        name[count++] = ch;                // ...store them in name.

        if (count == length - 1)           // Check for name full
            break;
    }

    name[count] = '\0';                  // Append string terminator

    if (count < length - 1)              // If we didn't end for name full...
        ungetc(ch, stdin);                 // ...return non-letter to stream

    return name;
}

// Function to check for newline
bool isnewline(void)
{
    char ch = 0;

    if ((ch = (char)getchar()) == '\n')
        return true;

    ungetc(ch, stdin);                   // Not newline so put it back
    return false;
}
