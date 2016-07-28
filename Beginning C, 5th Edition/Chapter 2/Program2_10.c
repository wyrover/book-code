// Program 2.10 Round tables again but shorter
#include <stdio.h>

int main(void)
{
    float diameter = 0.0f;                    // The diameter of a table
    float radius = 0.0f;                      // The radius of a table
    const float Pi = 3.14159f;                // Defines the value of Pi as fixed
    printf("Input the diameter of the table:");
    scanf("%f", &diameter);
    radius = diameter / 2.0f;
    printf("The circumference is %.2f. ", 2.0f * Pi * radius);
    printf("The area is %.2f.\n", Pi * radius * radius);
    return 0;
}
