// Exercise 10.3 A function to output double values in a given width.
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_COUNT 100

void show(double array[], size_t array_size, unsigned int field_width);

int main(void)
{
    double array[MAX_COUNT] = {0.0};
    size_t count = 0;

    for (double x = 1.5 ; x < 4.6 ; x += 0.3)
        array[count++] = x;

    unsigned int width = 12;
    show(array, count, width);
    printf_s("\n");
}

void show(double array[], size_t array_size, unsigned int field_width)
{
    char format[10] = {'\0'};                         // Holds the format string
    unsigned int places = 2;
    snprintf_s(format, sizeof(format), "%%%u.%ulf", field_width, places);

    // Output the values five to a line
    for (size_t j = 0 ; j < array_size ; ++j) {
        if (j % 5 == 0)
            printf_s("\n");

        printf_s(format, array[j]);
    }
}
