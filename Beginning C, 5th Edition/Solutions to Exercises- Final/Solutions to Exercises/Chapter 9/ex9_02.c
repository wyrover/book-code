// Exercise 9.2 Implementing arithmetic and array functions
#include <stdio.h>

double add(double a, double b);                                         // Returns a+b
double subtract(double a, double b);                                    // Returns a-b
double multiply(double a, double b);                                    // Returns a*b
double array_op(double array[], size_t size, double (*pfun)(double, double));


int main(void)
{
    double array[] = {11.0, 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    int length = sizeof array / sizeof(double);
    printf("The value of:\n");

    for (int i = 0 ; i < length ; i++) {
        printf("%.2lf%s", array[i], (i < length - 1 ? " + " : "\n"));
    }

    printf(" is %.2lf\n", array_op(array, length, add));
    printf("\nThe value of:\n");

    for (int i = 0 ; i < length ; i++) {
        printf("%.2lf%s", array[i], (i < length - 1 ? (i % 2 == 0 ? " - " : " + ") : "\n"));
    }

    printf(" is %.2lf\n", array_op(array, length, subtract));
    printf("\nThe value of:\n");

    for (int i = 0 ; i < length ; ++i) {
        printf("%.2lf%s", array[i], (i < length - 1 ? " * "  : "\n"));
    }

    printf(" is %.2lf\n", array_op(array, length, multiply));
    return 0;
}

// Function to calculate a+b
double add(double a, double b)
{
    return a + b;
}

// Function to calculate a-b
double subtract(double a, double b)
{
    return a - b;
}

// Function to calculate a*b
double multiply(double a, double b)
{
    return a * b;
}

// Function to apply an operation, pfun, between successive pairs of elements
double array_op(double array[], size_t size, double (*pfun)(double, double))
{
    double result = array[size - 1];
    size_t i = 0;

    // Work from last to first to accommodate alternating signs for subtract
    for (i = size - 1 ; i > 0 ; i--)
        result = pfun(array[i - 1], result);

    return result;
}
