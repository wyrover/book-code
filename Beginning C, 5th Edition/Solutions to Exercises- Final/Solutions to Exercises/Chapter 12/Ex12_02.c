// Exercise 12.2 Reading strings from a file in reverse order.
#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFFER_SIZE 50

// Function prototypes
size_t get_count(char *file_name);
fpos_t* get_string_positions(char *file_name, size_t str_count);
void copy_file_reversed(char *in_name, char *out_name, fpos_t *positions, size_t str_count);
void list_strings(char *file_name);

// Global variables
char *buffer = NULL;                               // Input/Output buffer for strings
size_t buffer_size = 0;                            // I/O buffer size

int main(void)
{
    char *infilename = "myfile.bin";                 // Name of the file to be read
    char *outfilename = "outfile.bin";               // Name of the file to be written
    buffer_size = BUFFER_SIZE;                       // Initial buffer size
    buffer = (char*)malloc(buffer_size);             // Create initial buffer
    size_t str_count = get_count(infilename);
    fpos_t *positions = get_string_positions(infilename, str_count);
    copy_file_reversed(infilename, outfilename, positions, str_count);
    list_strings(outfilename);
    // Free the memory we allocated
    free(buffer);
    free(positions);
    return 0;
}

// Function to get the number of strings in the file
// and set the buffer size to accommodate the maximum string length
size_t get_count(char *file_name)
{
    FILE *pFile = NULL;                                     // File pointer to input file
    size_t str_length = 0;                                  // Length of a string
    size_t str_count = 0;                                   // Number of strings

    if (fopen_s(&pFile, file_name, "rb")) {                 // Open the input file
        printf_s("Error opening %s for reading. Program terminated.", file_name);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);

    for (;;) {
        fread(&str_length, sizeof(size_t), 1, pFile);         // Read the string length

        if (feof(pFile))                                      // If it is end of file
            break;                                              // We are finished

        // Check buffer is large enough and increase if necessary
        if (str_length >= buffer_size) {
            buffer_size = str_length  + 1;

            if (!(buffer = (char*)realloc(buffer, buffer_size))) {
                printf_s("Buffer allocation failed.\n");
                exit(1);
            }
        }

        fread(buffer, str_length, 1, pFile);                  // Read the string
        ++str_count;
    }

    fclose(pFile);
    printf_s("\nThere are %zd strings in the input file.", str_count);
    return str_count;
}

// Function to get the position for the beginning of each record in the file
fpos_t* get_string_positions(char *file_name, size_t str_count)
{
    FILE *pFile = NULL;                                     // File pointer to input file

    if (fopen_s(&pFile, file_name, "rb")) {                 // Open the input file
        printf_s("Error opening %s for reading. Program terminated.", file_name);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);
    fpos_t *positions = (fpos_t*)malloc(str_count * sizeof(fpos_t)); // Array to store the positions
    size_t str_length = 0;                                  // Length of a string

    for (int i = 0 ; i < str_count ; ++i) {
        fgetpos(pFile, positions + i);                        // Get the positions
        fread(&str_length, sizeof(size_t), 1, pFile);         // Read the string length
        fread(buffer, str_length, 1, pFile);                  // Read the string
    }

    fclose(pFile);
    return positions;
}

// Copy strings from input file to new file in reverse order
void copy_file_reversed(char *in_name, char *out_name, fpos_t *positions, size_t str_count)
{
    FILE *pInFile = NULL;                                   // File pointer to input file
    FILE *pOutFile = NULL;                                  // File pointer  to output file

    // Open input file for binary read
    if (fopen_s(&pInFile, in_name, "rb")) {                 // Open the input file
        printf_s("Error opening %s for reading. Program terminated.", in_name);
        exit(1);
    }

    setvbuf(pInFile, NULL, _IOFBF, 512);

    // Open output file for binary write
    if (fopen_s(&pOutFile, out_name, "wb")) {
        printf_s("Error opening %s for writing. Program terminated.", out_name);
        exit(1);
    }

    setvbuf(pOutFile, NULL, _IOFBF, 512);
    // Read the records in reverse order from the input file and write to the new file
    size_t str_length = 0;

    for (int i = 0 ; i < str_count ; ++i) {
        fsetpos(pInFile, positions + str_count - i - 1);      // Set the file position
        fread(&str_length, sizeof(size_t), 1, pInFile);       // Read the string length
        fwrite(&str_length, sizeof(size_t), 1, pOutFile);     // Write to new file
        fread(buffer, str_length, 1, pInFile);                // Read the string
        fwrite(buffer, str_length, 1, pOutFile);              // Write to new file
    }

    fclose(pInFile);                                        // Close input file
    fclose(pOutFile);                                       // Close output file
    printf_s("\nNew file write complete.\n");
}

void list_strings(char *file_name)
{
    FILE *pFile = NULL;
    size_t str_length = 0;

    if (fopen_s(&pFile, file_name, "r")) {                  // Open the new file to read it
        printf_s("Error opening %s for reading. Program terminated.\n", file_name);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);
    printf_s("\nThe strings in the new file are:\n");

    while (true) {
        fread(&str_length, sizeof(size_t), 1, pFile);

        if (feof(pFile))                                      // If it is end of file
            break;                                              // We are finished

        fread(buffer, str_length, 1, pFile);
        buffer[str_length] = '\0';
        printf_s("\n%s", buffer);
    }

    printf_s("\n");
    fclose(pFile);                                          // Close file
}
