// Program 12.2  As the saying goes...it comes back!
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 81                           // Maximum input length

int main(void)
{
    char *proverbs[] = {
        "Many a mickle makes a muckle.\n",
        "Too many cooks spoil the broth.\n",
        "He who laughs last didn't get the joke in"
        " the first place.\n"
    };
    char more[LENGTH];                        // Stores a new proverb
    FILE *pfile = NULL;                       // File pointer
    char *filename = "myfile.txt";

    // Create a new file( if myfile.txt does not exist
    if (fopen_s(&pfile, filename, "w")) {      // Open the file to write it
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setbuf(pfile, NULL);

    // Write our locally stored proverbs to the file.
    for (size_t i = 0 ; i < sizeof proverbs / sizeof proverbs[0] ; ++i)
        if (EOF == fputs(proverbs[i], pfile)) {
            printf_s("Error writing file.\n");
            exit(1);
        }

    fclose(pfile);                            // Close the file

    // Open the file to append more proverbs
    if (fopen_s(&pfile, filename, "a")) {
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setbuf(pfile, NULL);
    printf_s("Enter proverbs of up to 80 characters or press Enter to end:\n");

    while (true) {
        fgets(more, LENGTH, stdin);               // Read a proverb

        if (more[0] == '\n')                      // If its empty line
            break;                                  // end input operation

        if (EOF == fputs(more, pfile)) {          // Write the new proverb
            printf_s("Error writing file.\n");
            exit(1);
        }
    }

    fclose(pfile);                              // Close the file

    if (fopen_s(&pfile, filename, "r")) {      // Open the file to read it
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setbuf(pfile, NULL);
    // Read and output the file contents
    printf_s("The proverbs in the file are:\n");

    while (fgets(more, LENGTH, pfile))          // Read a proverb
        printf_s("%s", more);                      // and display it

    fclose(pfile);                              // Close the file
    remove(filename);                           // and remove it
    return 0;
}

