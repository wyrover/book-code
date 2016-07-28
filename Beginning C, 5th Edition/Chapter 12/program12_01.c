// Program 12.1 Writing a file a character at a time
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LENGTH 81                     // Maximum input length

int main(void)
{
    char mystr[LENGTH];                 // Input string
    int mychar = 0;                     // Character for output
    FILE *pfile = NULL;                 // File pointer
    char *filename = "myfile.txt";
    printf("Enter an interesting string of up to 80 characters:\n");

    if (!fgets(mystr, LENGTH, stdin)) { // Read in a string
        printf_s("Input from keyboard failed.\n");
        exit(1);
    }

    // Create a new file we can write
    if (fopen_s(&pfile, filename, "w")) {
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setvbuf(pfile, NULL, _IOFBF, BUFSIZ);  // Buffer the file

    for (int i = strlen(mystr) - 1 ; i >= 0 ; --i)
        fputc(mystr[i], pfile);           // Write string to file backward

    fclose(pfile);                      // Close the file

    // Open the file for reading
    if (fopen_s(&pfile, filename, "r")) {
        printf_s("Error opening %s for reading. Program terminated.", filename);
        exit(1);
    }

    setvbuf(pfile, NULL, _IOFBF, BUFSIZ);  // Buffer the file
    // Read a character from the file and display it
    printf_s("the data read from the file is:\n");

    while ((mychar = fgetc(pfile)) != EOF)
        putchar(mychar);                  // Output character from the file

    putchar('\n');                      // Write newline
    fclose(pfile);                      // Close the file
    remove(filename);                   // Delete the physical file
    return 0;
}

