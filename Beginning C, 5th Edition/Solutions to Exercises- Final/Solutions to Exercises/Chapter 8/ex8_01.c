// Exercise 8.1 A function to calculate an average
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CAPACITY_INCREMENT 6                     // Increment in the capacity for data values

double average(double data[], int count)
{
    double sum = 0.0;

    for (int i = 0 ; i < count ; sum += data[i++])
        ;

    return sum / count;
}

int main(void)
{
    double *data = NULL;                          // Pointer to array of data values
    double *temp = NULL;                          // Pointer to new array of data values
    int count = 0;                                // Number of data values
    int capacity = 0;                             // Number of data values that can be stored
    char answer = 'n';

    do {
        if (count == capacity) {
            capacity += CAPACITY_INCREMENT;

            // Create new array of pointers
            if (!(temp = (double*)realloc(data, capacity * sizeof(double)))) {
                printf("Error allocating memory for data values.\n");
                exit(1);
            }

            data = temp;
        }

        printf("Enter a data value: ");
        scanf_s(" %lf", data + count++);
        printf("Do you want to enter another (y or n)? ");
        scanf_s(" %c", &answer, sizeof(answer));
    } while (tolower(answer) != 'n');

    printf("\nThe  average of the values you entered is %10.2lf\n", average(data, count));
    free(data);
    return 0;
}

