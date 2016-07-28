// Program 7.1 A simple program using pointers
#include <stdio.h>

int main(void)
{
    int number = 0;                                         // A variable of type int initialized to 0
    int *pnumber = NULL;                                    // A pointer that can point to type int
    number = 10;
    printf("number's address: %p\n", &number);              // Output the address
    printf("number's value: %d\n\n", number);               // Output the value
    pnumber = &number;                                      // Store the address of number in pnumber
    printf("pnumber's address: %p\n", (void*)&pnumber);     // Output the address
    printf("pnumber's size: %zd bytes\n", sizeof(pnumber)); // Output the size
    printf("pnumber's value: %p\n", pnumber);               // Output the value (an address)
    printf("value pointed to: %d\n", *pnumber);             // Value at the address
    return 0;
}
