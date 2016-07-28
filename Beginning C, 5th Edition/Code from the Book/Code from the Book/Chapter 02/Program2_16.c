// Program 2.16 Using type char
#include <stdio.h>

int main(void)
{
    char first = 'A';
    char second = 'B';
    char last = 'Z';
    char number = 40;
    char ex1 = first + 2;                     // Add 2 to 'A'
    char ex2 = second - 1;                    // Subtract 1 from 'B'
    char ex3 = last + 2;                      // Add 2 to 'Z'
    printf("Character values      %-5c%-5c%-5c\n", ex1, ex2, ex3);
    printf("Numerical equivalents %-5d%-5d%-5d\n", ex1, ex2, ex3);
    printf("The number %d is the code for the character %c\n", number, number);
    return 0;
}
