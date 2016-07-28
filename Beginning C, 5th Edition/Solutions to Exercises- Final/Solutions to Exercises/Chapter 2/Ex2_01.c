// Exercise 2.1 Convert inches to yards, feet, and inches
#include <stdio.h>

int main(void)
{
    int inches = 0;
    int yards = 0;
    int feet = 0;
    const int inches_per_foot = 12;  // There are 12 inches in 1 foot.
    const int feet_per_yard = 3;     // Rhere are 3 feet in 1 yard.
    printf("Enter a distance in inches: ");
    scanf("%d", &inches);
    feet = inches / inches_per_foot; // Get whole feet.
    yards = feet / feet_per_yard;    // Get whole yards in value of feet.
    feet %= feet_per_yard;           // Get residual feet.
    inches %= inches_per_foot;       // Get residual inches.
    printf("That is equivalent to %d yards %d feet and %d inches.\n", yards, feet, inches);
    return 0;
}
