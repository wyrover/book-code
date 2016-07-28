// Exercise 3.1 Convert temperatures
#include <stdio.h>

int main(void)
{
    int choice = 0;
    double temperature = 0.0;
    printf("This program can do the following:\n"
           "1. Convert from degrees Centigrade to degrees Fahrenheit\n"
           "2. Convert from degrees Fahrenheit to degrees Centigrade\n"
           "Select the conversion (1 or 2): ");
    scanf("%d", &choice);
    printf("Enter a temperature in degrees %s: ",
           (choice == 1 ?  "Centigrade" : "Fahrenheit"));
    scanf("%lf", &temperature);

    if (choice == 1)
        printf("That is equivalent to %.2lf degrees Fahrenheit\n", temperature * 9.0 / 5.0 + 32.0);
    else
        printf("That is equivalent to %.2lf degrees Centigrade\n", (temperature - 32.0) * 5.0 / 9.0);

    return 0;
}
