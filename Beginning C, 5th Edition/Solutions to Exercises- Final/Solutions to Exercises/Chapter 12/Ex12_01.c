// Exercise 12.1 Writing strings to a file.
/*
  This program creates the file in the current directory.
  You will nee to copy or move the file to the current directory for Exercise 12.2.
  Alternatively you can amend the code to specify a full file path and use the
  same path in Exercise 12.2.
*/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 50

// Function prototypes
char *read_string(char *buffer, int *pbuffer_size);

int main(void)
{
    FILE *pFile = NULL;                               // File pointer
    char *filename = "myfile.bin";                    // Name of the file to be written
    char answer = 'n';
    size_t str_length = 0;
    int buffer_size = BUFFER_SIZE;
    char *buffer = malloc(buffer_size);               // Create initial buffer

    if (fopen_s(&pFile, filename, "wb")) {            // Open myfile.txt to write it
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);

    do {
        printf_s("Enter a string:\n");
        read_string(buffer, &buffer_size);              // Read a string into buffer
        str_length = strnlen_s(buffer, buffer_size);    // Get the string length
        fwrite(&str_length, sizeof(size_t), 1, pFile);  // Write string length to file
        fwrite(buffer, str_length, 1, pFile);           // Write string to file
        printf_s("Do you want to enter another (y or n)? ");
        scanf_s(" %c", &answer, sizeof(answer));
        fflush(stdin);                                  // Flush to lose the newline
    } while (tolower(answer) == 'y');

    fclose(pFile);                                    // Close file
    printf_s("\nFile write complete\n");

    if (buffer)
        free(buffer);

    return 0;
}

// Reads a string of arbitrary length from the keyboard
// If the string exceeds the buffer capacity, the buffer
// is increased automatically.
char *read_string(char *buffer, int *pbuffer_size)
{
    char *temp = NULL;                                // Used to hold new buffer address
    int position = 0;                                 // Current free position in buffer

    while ((buffer[position++] = getchar()) != '\n') {
        if (position >= *pbuffer_size - 1) {            // Is the buffer full?
            // Increase the size of the buffer
            *pbuffer_size += BUFFER_SIZE;                 // New buffer size

            if (!(temp = (char*)realloc(buffer, *pbuffer_size))) { // Create new buffer
                printf_s("Error allocatiuong input buffer.\n");
                exit(1);
            }

            position = temp + position - buffer;
            buffer = temp;
        }
    }

    buffer[position - 1] = '\0';                      // Overwrite newline with terminator
    return buffer;
}
