// Program 7.10  Understand pointers to your hat size - if you dare
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    char size[3][12] = {                // Hat sizes as characters
        {'6', '6', '6', '6', '7', '7', '7', '7', '7', '7', '7', '7'},
        {'1', '5', '3', '7', ' ', '1', '1', '3', '1', '5', '3', '7'},
        {'2', '8', '4', '8', ' ', '8', '4', '8', '2', '8', '4', '8'}
    };
    int headsize[12] =                  // Values in 1/8 inches
    {164, 166, 169, 172, 175, 178, 181, 184, 188, 191, 194, 197};
    char *psize = *size;
    int *pheadsize = headsize;
    float cranium = 0.0f;               // Head circumference in decimal inches
    int your_head = 0;                  // Headsize in whole eighths
    bool hat_found = false;             // Indicates when a hat is found to fit
    // Get the circumference of the head
    printf("\nEnter the circumference of your head above your eyebrows"
           " in inches as a decimal value: ");
    scanf_s(" %f", &cranium);
    your_head = (int)(8.0f * cranium);  // Convert to whole eighths of an inch
    /*****************************************************************
     * Search for a hat size:                                        *
     * Either your head corresponds to the 1st headsize element or   *
     * a fit is when your_head is greater that one headsize element  *
     * and less than or equal to the next.                           *
     * In this case the size is the second headsize value.           *
     *****************************************************************/
    unsigned int i = 0;                                                     // Loop counter

    if (your_head == *pheadsize)                                            // Check for min size fit
        hat_found = true;
    else {
        // Find head size in the headsize array
        for (i = 1 ; i < sizeof(headsize) / sizeof(*headsize) ; ++i) {
            if (your_head > *(pheadsize + i - 1) && your_head <= *(pheadsize + i)) {
                hat_found = true;
                break;
            }
        }
    }

    if (hat_found) {
        printf("\nYour hat size is %c %c%c%c\n",
               *(psize + i),                                       // First row of size
               * (psize + 1 * sizeof(*size) / sizeof(**size) + i), // Second row of size
               (*(psize + 1 * sizeof(*size) / sizeof(**size) + i) == ' ') ? ' ' : '/',
               *(psize + 2 * sizeof(*size) / sizeof(**size) + i)); // Third row of size
    }
    // If no hat was found, the head is too small, or too large
    else {
        if (your_head < *pheadsize)                                           // check for too small
            printf("\nYou are the proverbial pinhead. No hat for"
                   " you I'm afraid.\n");
        else                                                                  // It must be too large
            printf("\nYou, in technical parlance, are a fathead."
                   " No hat for you, I'm afraid.\n");
    }

    return 0;
}
