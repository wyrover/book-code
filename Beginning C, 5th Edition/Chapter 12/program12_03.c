// Program 12.3 Messing about with formatted file I/O
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    long num1 = 234567L;                                          // Input values...
    long num2 = 345123L;
    long num3 = 789234L;
    long num4 = 0L;                                               // Values read from the file...
    long num5 = 0L;
    long num6 = 0L;
    float fnum = 0.0f;                                            // Value read from the file
    int   ival[6] = { 0 };                                        // Values read from the file
    FILE *pfile = NULL;                                           // File pointer
    char *filename = "myfile.txt";

    if (fopen_s(&pfile, filename, "w")) {
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setbuf(pfile, NULL);
    fprintf_s(pfile, "%6ld%6ld%6ld", num1, num2, num3);           // Write file
    fclose(pfile);                                                // Close file
    printf_s(" %6ld %6ld %6ld\n", num1, num2, num3);              // Display values written

    if (fopen_s(&pfile, filename, "r")) {
        printf_s("Error opening %s for reading. Program terminated.\n", filename);
        exit(1);
    }

    setbuf(pfile, NULL);
    fscanf_s(pfile, "%6ld%6ld%6ld", &num4, &num5 , &num6);        // Read back
    printf_s(" %6ld %6ld %6ld\n", num4, num5, num6);              // Display what we got
    rewind(pfile);                                                // Go to the beginning of the file
    fscanf_s(pfile, "%2d%3d%3d%3d%2d%2d%3f", &ival[0], &ival[1],  // Read it again
             &ival[2], &ival[3], &ival[4] , &ival[5], &fnum);
    fclose(pfile);                                                // Close the file and
    remove(filename);                                             // delete physical file.
    // Output the results
    printf_s("\n");

    for (size_t i = 0 ; i < sizeof(ival) / sizeof(ival[0]) ; ++i)
        printf_s("%sival[%zd] = %d", i == 4 ? "\n\t" : "\t", i, ival[i]);

    printf_s("\nfnum = %f\n", fnum);
    return 0;
}

