// Program 6.2 Lengths of strings
#include <stdio.h>
int main(void)
{
    char str1[] = "To be or not to be";
    char str2[] = ",that is the question";
    unsigned int count = 0;                // Stores the string length

    while (str1[count] != '\0')            // Increment count till we reach the
        ++count;                             //  terminating character.

    printf("The length of the string \"%s\" is %d characters.\n", str1, count);
    count = 0;                             // Reset count for next string

    while (str2[count] != '\0')            // Count characters in second string
        ++count;

    printf("The length of the string \"%s\" is %d characters.\n", str2, count);
    return 0;
}
