//Exercise 5.3 Handling monetary values as integers
#include <stdio.h>

int main(void)
{
    const size_t size = 5;
    float amounts[size];                     // Stores data values
    long dollars[size];
    long cents[size];
    printf("Enter %zd monetary values separated by spaces:\n", size);

    for (size_t i = 0 ; i < size ; ++i)
        scanf("%f", &amounts[i]);

    for (size_t i = 0 ; i < size ; ++i) {
        dollars[i] = (long)amounts[i];
        cents[i] = (long)(100.0 * (amounts[i] - dollars[i]));
    }

    printf("\n");

    for (size_t i = 0 ; i < size ; ++i)
        printf("  $%d.%02d", dollars[i], cents[i]);

    printf("\n");
    return 0;
}

