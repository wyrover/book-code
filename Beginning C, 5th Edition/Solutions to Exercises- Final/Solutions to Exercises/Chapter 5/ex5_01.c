//Exercise 5.1 Summing reciprocals of five values
#include <stdio.h>

int main(void)
{
    const int nValues = 5;               // Number of data values
    double data[nValues];                // Stores data values
    double reciprocals[nValues];
    double sum = 0.0;                    // Stores sum of reciprocals
    printf("Enter five values separated by spaces: \n");

    for (int i = 0 ; i < nValues ; ++i)
        scanf("%lf", &data[i]);

    printf("You entered the values:\n");

    for (int i = 0 ; i < nValues ; ++i)
        printf("%15.2lf", data[i]);

    printf("\n");
    printf("\nThe values of the reciprocals are:\n");

    for (int i = 0 ;  i < nValues ; ++i) {
        reciprocals[i] = 1.0 / data[i];
        printf("%15.2lf", reciprocals[i]);
    }

    printf("\n\n");

    for (int i = 0 ; i < nValues ; i++) {
        sum += reciprocals[i];              // Accumulate sum of reciprocals

        if (i > 0)
            printf(" + ");

        printf("1/%.2lf", data[i]);
    }

    printf(" = %lf\n", sum);
    return 0;
}

