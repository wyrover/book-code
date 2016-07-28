// Exercise 2.2  Calculating the area of a room
#include <stdio.h>

int main(void)
{
    double length = 0.0;                     // Room length in yards
    double width = 0.0;                      // Room width in yards
    long feet = 0L;                          // A whole number of feet
    long inches = 0L;                        // A whole number of inches
    const long inches_per_foot = 12L;
    const double inches_per_yard = 36L;
    // Get the length of the room
    printf("Enter the length of the room in feet and inches - whole feet first: ");
    scanf("%ld", &feet);
    printf("                                           ...Now enter the inches: ");
    scanf("%ld", &inches);
    length = (feet * inches_per_foot + inches) / inches_per_yard;
    // Get the width of the room
    printf("Enter the width of the room in feet and inches - whole feet first: ");
    scanf("%ld", &feet);
    printf("                                          ...Now enter the inches: ");
    scanf("%ld", &inches);
    width = (feet * inches_per_foot + inches) / inches_per_yard;
    // Output the area
    printf("The area of the room is %.2f square yards.\n", length * width);
    return 0;
}
