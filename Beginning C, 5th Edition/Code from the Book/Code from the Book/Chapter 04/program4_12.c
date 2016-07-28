// Program 4.12 Reversing the digits
#include <stdio.h>
int main(void)
{
    unsigned int number = 0;                 // The number to be reversed
    unsigned int rebmun = 0;                 // The reversed number
    unsigned int temp = 0;                   // Working storage
    // Read in the value to be reversed
    printf("\nEnter a positive integer: ");
    scanf(" %u", &number);
    temp = number;                           // Copy to working storage

    // Reverse the number stored in temp
    do {
        rebmun = 10 * rebmun + temp % 10;      // Add rightmost digit of temp to rebmun
        temp = temp / 10;                      // and remove it from temp
    } while (temp);                          // Continue as long as temp is not 0

    printf("\nThe number %u reversed is  %u rebmun ehT\n", number, rebmun);
    return 0;
}
