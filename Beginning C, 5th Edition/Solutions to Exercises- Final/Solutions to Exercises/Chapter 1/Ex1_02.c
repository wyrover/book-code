// Exercise 1.2 Output a name and address in a single statement
#include <stdio.h>

int main(void)
{
    // The compiler will automatically join strings together into
    // a single string when they immediately follow one another
    // so the three strings between the brackets below will
    // be assembled into one long string.
    // Each \n will cause what follows to be on a new line.
    printf("George Washington\n"
           "3200 George Washington Memorial Parkway\n"
           "Mount Vernon\nVirginia 22121\n");
    return 0;
}
