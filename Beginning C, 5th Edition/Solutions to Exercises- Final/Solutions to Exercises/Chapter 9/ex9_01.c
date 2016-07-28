// Exercise 9.1 A recursive power function
#include <stdio.h>

double power(double x, int n);


int main(void)
{
    for (double x = 2.0 ; x <= 5.0; x += 0.5) {
        for (int n = -4 ; n < 5 ; ++n)
            printf("The value of %.2lf raised to the power %d is %.4lf\n", x, n, power(x, n));

        printf("\n");
    }

    return 0;
}

// Function to raise x to the power n
double power(double x, int n)
{
    if (n < 0) {
        n = -n;
        x = 1.0 / x;
    }

    if (n == 0)
        return 1.0;

    return x * power(x, n - 1);
}
