// Exercise 2.4  Calculating average hourly pay rate
#include <stdio.h>

int main(void)
{
    double pay = 0.0;                // Weekly pay in dollars
    double hours = 0.0;              // hours worked
    int dollars = 0;                 // Hourly rate - dollars
    int cents = 0;                   // ... and cents
    // Get the Weekly pay
    printf("Enter your weekly pay in dollars: ");
    scanf("%lf", &pay);
    // Get the order quantity
    printf("Enter the hours worked: ");
    scanf("%lf", &hours);
    // Calculate the average hourly rate - dollars first
    dollars = (int)(pay / hours);
    // to get the cents we can subtract the dollars from the hourly rate
    // and multiply by 100 to get cents. If we then add 0.5 and convert the result
    // back to an integer, it will be to the nearest cent.
    cents = (int)(100.0 * (pay / hours - dollars) + 0.5);
    // Output the average hourly rate
    printf("Your average hourly pay rate is %d dollars and %d cents.\n", dollars, cents);
    return 0;
}
