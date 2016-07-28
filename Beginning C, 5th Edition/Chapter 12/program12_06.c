// Program 12.6 Writing a binary file with an update mode
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN  50                                    // Size of name buffer

void listfile(const char *filename);                  // List the file contents

int main(void)
{
    const char *filename = "mypeople.bin";
    char name[MAXLEN];                                  // Stores a name
    size_t length = 0;                                  // Length of a name
    int age = 0;                                        // Person's age
    char answer = 'y';
    FILE *pfile = NULL;

    if (fopen_s(&pfile, filename, "wb+")) {
        printf_s("Failed to create file %s.\n", filename);
        exit(1);
    }

    do {
        fflush(stdin);                                    // Remove whitespace
        printf_s("Enter a name less than %d characters: ", MAXLEN);
        gets_s(name, sizeof(name));                       // Read the name
        printf_s("Enter the age of %s: ", name);
        scanf_s(" %d", &age);                             // Read the age
        // Write the name & age to file
        length = strnlen_s(name, sizeof(name));           // Get name length
        fwrite(&length, sizeof(length), 1, pfile);        // Write name length
        fwrite(name, sizeof(char), length, pfile);        // then the name
        fwrite(&age, sizeof(age), 1, pfile);              // then the age
        printf_s("Do you want to enter another(y or n)?  ");
        scanf_s("\n%c", &answer, sizeof(answer));
    } while (tolower(answer) == 'y');

    fclose(pfile);                                      // Close the file
    listfile(filename);                                 // List the contents
    return 0;
}

// List the contents of the binary file
void listfile(const char *filename)
{
    size_t length = 0;                                  // Name length
    char name[MAXLEN];                                  // Stores a name
    int age = 0;
    char format[20];                                    // Format string
    FILE *pfile = NULL;
    // Create format string for names up to MAXLEN characters
    sprintf_s(format, sizeof(format), "%%-%ds Age:%%4d\n", MAXLEN);

    if (fopen_s(&pfile, filename, "rb")) {              // Open to read
        printf_s("Failed to open file %s to read it.\n", filename);
        exit(1);
    }

    printf_s("\nThe folks recorded in the %s file are:\n", filename);

    // Read records as long as we read a length value
    while (fread(&length, sizeof(length), 1, pfile) == 1) {
        if (length + 1 > MAXLEN) {
            printf_s("Name too long.\n");
            exit(1);
        }

        fread(name, sizeof(char), length, pfile);         // Read the name
        name[length] = '\0';                              // Append terminator
        fread(&age, sizeof(age), 1, pfile);               // Read the age
        printf_s(format, name, age);                      // Output the record
    }

    fclose(pfile);
}

