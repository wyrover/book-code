// Program 3.10 Exercising bitwise operators
#include <stdio.h>

int main(void)
{
    unsigned int original = 0xABC;
    unsigned int result = 0;
    unsigned int mask = 0xF;    // Rightmost four bits
    printf("\n original = %X", original);
    // Insert first digit in result
    result |= original & mask;  // Put right 4 bits from original in result
    // Get second digit
    original >>= 4;             // Shift original right four positions
    result <<= 4;               // Make room for next digit
    result |= original & mask;  // Put right 4 bits from original in result
    /* Get third digit */
    original >>= 4;             // Shift original right four positions
    result <<= 4;               // Make room for next digit
    result |= original & mask;  // Put right 4 bits from original in result
    printf("\t result = %X\n", result);
    return 0;
}
