// Program 12.8 Viewing the contents of a file
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN     256                               // Maximum file path length
#define DISPLAY     80                               // Length of display line
#define PAGE_LENGTH 20                               // Lines per page

int main(int argc, char *argv[])
{
    char filename[MAXLEN];                              // Stores the file path
    FILE *pfile = NULL;                                 // File pointer
    unsigned char buffer[DISPLAY / 4 - 1];              // File input buffer
    size_t count = 0;                                   // Count of characters in buffer
    int lines = 0;                                      // Number of lines displayed

    if (argc == 1) {                                    // No file name on command line?
        printf_s("Please enter a filename: ");            // Prompt for input
        fgets(filename, MAXLEN, stdin);                   // Get the file name entered
        // Remove the newline if it's there
        int len = strnlen_s(filename, sizeof(filename));

        if (filename[len - 1] == '\n')
            filename[len - 1] = '\0';
    } else
        strcpy(filename, argv[1]);                        // Get 2nd command line string

    if (fopen_s(&pfile, filename, "rb")) {              // Open for binary read
        printf_s("Sorry, can't open %s.\n", filename);
        return -1;
    }

    setbuf(pfile, NULL);                                // Buffer file input

    while (!feof(pfile)) {                              // Continue until end of file
        if (count < sizeof(buffer))                       // If the buffer is not full
            buffer[count++] = (unsigned char)fgetc(pfile);  // Read a character
        else {
            // Output the buffer contents, first as hexadecimal
            for (size_t i = 0 ; i < sizeof(buffer) ; ++i)
                printf_s("%02X ", buffer[i]);

            printf_s("| ");                                 // Output separator

            // Now display buffer contents as characters
            for (size_t i = 0 ; i < sizeof(buffer) ; ++i)
                printf_s("%c", isprint(buffer[i]) ? buffer[i] : '.');

            printf_s("\n");                                 // End the line
            count = 0;                                      // Reset count

            if (!(++lines % PAGE_LENGTH))                   // End of page?
                if (toupper(getchar()) == 'E')                 // Wait for Enter
                    return 0;                                    // E pressed
        }
    }

    // Display the last line if there is one...
    if (count > 0) {
        for (size_t i = 0 ; i < sizeof(buffer) ; ++i)     //...first as hexadecimal
            if (i < count)
                printf_s("%02X ", buffer[i]);                 // Output hexadecimal
            else
                printf_s("   ");                              // Output spaces

        printf_s("| ");                                   // Output separator

        for (size_t i = 0 ; i < count ; ++i)              // Output characters
            printf_s("%c", isprint(buffer[i]) ? buffer[i] : '.');

        printf_s("\n");                                   // End the line
    }

    fclose(pfile);                                      // Close the file
    return 0;
}

