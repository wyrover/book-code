//Exercise 5.4 Table of reciprocals, squares, cubes, and fourth powers.
#include <stdio.h>

int main(void)
{
    const size_t nrows = 11;              // Number of rows in the array
    const size_t ncols = 5;               // Number of columns in the array
    double data[nrows][ncols];            // Stores data values
    double value = 2.0;                   // Value to be stored in array

    for (size_t row = 0 ; row < nrows ; ++row) {
        data[row][0] = value;
        data[row][1] = 1.0 / data[row][0];             // 1/x
        data[row][2] = data[row][0] * data[row][0];    // x*x
        data[row][3] = data[row][2] * data[row][0];    // x*x*x
        data[row][4] = data[row][3] * data[row][0];    // x*x*x*x
        value += 0.1;
    }

    printf("            x  ");
    printf("          1/x  ");
    printf("          x*x  ");
    printf("         x*x*x ");
    printf("        x*x*x*x");

    for (size_t row = 0 ; row < nrows ; ++row) {
        printf("\n");

        for (size_t col = 0 ; col < ncols ; ++col)
            printf("%15.4lf", data[row][col]);
    }

    printf("\n");
    return 0;
}

