// Exercise 7.4 Read and process daily temperatures
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DAYS             2                                 // Initial number of days allowed for
#define READINGS_PER_DAY 6                                 // Readings per day

int main(void)
{
    double **data = NULL;                                   // Pointer to array of pointers to arrays of temperatures
    double **newdata = NULL;                                // Pointer to array of pointers to arrays of temperatures
    double *averages = NULL;                                // Pointer to array of averages
    int day = 0;
    int max_days = 0;
    char answer = 'n';

    do {
        if (day == max_days) {
            max_days += DAYS;
            // Create new array of pointers
            newdata = (double**)malloc(max_days * sizeof(double*));

            // Create an array of values of type double for each new day and store the address
            for (int i = day ; i < max_days ; ++i)
                *(newdata + i) = (double*)malloc(READINGS_PER_DAY * sizeof(double));

            if (data != NULL) {
                // Copy the addresses of the existing arrays of temperatures
                for (int i = 0 ; i < day ; ++i)
                    *(newdata + i) = *(data + i);

                free(data);                                       // Free memory for the old array of pointers
            }

            data = newdata;                                     // copy the address of the new array of pointers
            newdata = NULL;                                     // Reset the pointer
        }

        printf("Enter the %d readings for a day:\n", READINGS_PER_DAY);

        for (int i = 0 ; i < READINGS_PER_DAY ; ++i)
            scanf_s(" %lf", *(data + day) + i);

        ++day;
        printf("Do you want to enter another set(y or n)? ");
        scanf_s(" %c", &answer, sizeof(answer));
    } while (tolower(answer) != 'n');

    // If there is unused memory for temperature values - release it
    for (int i = day ; i < max_days ; ++i)
        free(*(data + i));

    // Allocate space for the averages */
    averages = (double*)malloc(day * sizeof(double));

    //Calculate the averages */
    for (int i = 0 ; i < day ; ++i) {
        *(averages + i) = 0.0;

        for (int j = 0 ; j < READINGS_PER_DAY ; ++j)
            *(averages + i) += *(*(data + i) + j);

        *(averages + i) /= READINGS_PER_DAY;
    }

    printf("\nThe daily temperatures and averages are:\n");

    for (int i = 0 ; i < day ; ++i) {
        for (int j = 0 ; j < READINGS_PER_DAY ; ++j)
            printf("%8.1lf", *(*(data + i) + j));

        printf("   Average:%8.1lf\n", *(averages + i));
    }

    // Finally - release all the heap memory
    free(averages);

    for (int i = 0 ; i < day ; ++i)
        free(*(data + i));

    free(data);
    return 0;
}

