// Program 7.3  Using pointer arguments to scanf
#include <stdio.h>

int main(void)
{
    int value = 0;
    int *pvalue = &value;                   // Set pointer to refer to value
    printf("Input an integer: ");
    scanf(" %d", pvalue);                   // Read into value via the pointer
    printf("You entered %d.\n", value);      // Output the value entered
    return 0;
}
