// Exercise 7.1 Calculating a floating-point average using pointers
/*********************************************************************
 * In this solution I allocate a some memory and when it is full     *
 * allocate a new, larger amount of memory and copy the contents of  *
 * the old memory to the new. I then free the old memory. This       *
 * process repeats as often as necessary.                            *
**********************************************************************/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define CAP_INCR 5                                               // Capacity increment for memory allocation

int main(void)
{
    double *values = NULL;                                         // Pointer to memory holding data values - none inmtially
    int capacity = 0;                                              // Maximum number of values that can be stored - none initially
    double *temp = NULL;                                           // Pointer to newly allocated memory
    double sum = 0.0;                                              // Sum of values
    int count = 0;                                                 // Number of values read
    char answer = 'n';

    do {
        if (count == capacity) {                                     // Check if there is spare memory
            capacity += CAP_INCR;                                      // Increase the capacity of memory by increment
            temp = realloc(values, capacity * sizeof(double));         // and reallocate it

            if (!temp) {                                               // If memory was not allocated
                // Output a message  and end
                printf("Memory allocation failed. Terminating program.");
                exit(1);
            }

            // When there are values stored in memory, they will be copied automatically to the new memory
            values = temp;                                              // Copy address of new memory to values
            temp = NULL;                                               // Reset pointer
        }

        printf("Enter a value: ");
        scanf_s("%lf", values + count++);
        printf("Do you want to enter another(y or n)? ");
        scanf(" %c", &answer);
    } while (tolower(answer) == 'y');

    // Now sum the values
    for (size_t i = 0 ; i < count ; ++i)
        sum += *(values + i);

    // Output the average
    printf("\nThe average of the the values you entered is %.2lf.\n", sum / count);
    free(values);
    // We are done - so free the memory
    return 0;
}

