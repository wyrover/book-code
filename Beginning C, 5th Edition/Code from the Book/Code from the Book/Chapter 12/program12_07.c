// Program 12.7 Writing, reading and updating a binary file
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 50                                        // Size of name buffer
const char *filename = "my-people.bin";                  // File name

// Structure encapsulating a name and age
typedef struct Record {
    char name[MAXLEN];
    int age;
} Record;

void list_file(void);                                    // List the file contents
void update_file(void);                                  // Update the file contents
Record *get_person(Record *precord);                     // Create a record from keyboard input
void get_name(char *pname, size_t size);                 // Read a name from stdin
void write_file(const char *mode);                       // Write records to a file
void write_record(const Record *precord, FILE *pfile);   // Write a file record
Record *read_record(Record *precord, FILE *pfile);       // Read a file record
int find_record(Record *precord, FILE *pfile);
void duplicate_file(const Record *pnewrecord, int index, FILE *pfile);

int main(void)
{
    // Choose activity
    char answer = 'q';

    while (true) {
        printf_s("Choose from the following options:\n"
                 "   To list the file contents enter  L\n"
                 "   To create a new file enter       C\n"
                 "   To add new records enter         A\n"
                 "   To update existing records enter U\n"
                 "   To delete the file enter         D\n"
                 "   To end the program enter         Q\n : ");
        scanf_s("\n%c", &answer, sizeof(answer));

        switch (toupper(answer)) {
        case 'L':                                          // List file contents
            list_file();
            break;

        case 'C':                                          // Create new file
            write_file("wb+");
            printf_s("\nFile creation complete.\n");
            break;

        case 'A':                                          // Append new record
            write_file("ab+");
            printf_s("\nFile append complete.\n");
            break;

        case 'U':                                          // Update existing records
            update_file();
            break;

        case 'D':
            printf_s("Are you sure you want to delete %s (y or n)? ", filename);
            scanf_s("\n%c", &answer, sizeof(answer));

            if (tolower(answer) == 'y')
                remove(filename);

            break;

        case 'Q':                                          // Quit the program
            printf_s("Ending the program.\n", filename);
            exit(0);

        default:
            printf_s("Invalid selection. Try again.\n");
            break;
        }
    }

    return 0;
}

// Read the name and age for a record from the keyboard
Record *get_person(Record *precord)
{
    printf_s("Enter a name less than %d characters: ", MAXLEN);
    get_name(precord->name, MAXLEN);                       // read the name
    printf_s("Enter the age of %s: ", precord->name);
    scanf_s(" %d", &precord->age);                         // Read the age
    return precord;
}

// Read a name from the keyboard
void get_name(char *pname, size_t size)
{
    fflush(stdin);
    fgets(pname, size, stdin);                             // Read the name
    size_t len = strnlen_s(pname, size);

    if (pname[len - 1] == '\n')                            // if there's a newline
        pname[len - 1] = '\0';                               // overwrite it
}

// Write a new record to the file at the current position
void write_record(const Record *precord, FILE *pfile)
{
    // Write the name & age to file
    size_t length = strnlen_s(precord->name, sizeof(precord->name));
    fwrite(&length, sizeof(length), 1, pfile);              // Write name length
    fwrite(precord->name, sizeof(char), length, pfile);     // ...then the name
    fwrite(&precord->age, sizeof(precord->age), 1, pfile);  // ...then the age
}

// Reads a record from the file at the current position
Record *read_record(Record *precord, FILE *pfile)
{
    size_t length = 0;                                     // Name length
    fread(&length, sizeof(length), 1, pfile);              // Read the length

    if (feof(pfile))                                       // If it's end file
        return NULL;

    fread(precord->name, sizeof(char), length, pfile);     // Read the name
    precord->name[length] = '\0';                          // Append terminator
    fread(&precord->age, sizeof(precord->age), 1, pfile);  // Read the age
    return precord;
}

// Write to a file
void write_file(const char *mode)
{
    char answer = 'y';
    FILE *pfile = NULL;

    if (fopen_s(&pfile, filename, mode)) {
        fprintf_s(stderr, "File open failed.\n");
        exit(1);
    }

    do {
        Record record;                                       // Stores a record name & age
        write_record(get_person(&record), pfile);            // Get record & write the file
        printf_s("Do you want to enter another(y or n)?  ");
        scanf_s(" %c", &answer, sizeof(answer));
        fflush(stdin);                                       // Remove whitespace
    } while (tolower(answer) == 'y');

    fclose(pfile);                                         // Close the file
}

// List the contents of the binary file
void list_file(void)
{
    // Create the format string for names up to MAXLEN long
    // format array length allows up to 5 digits for MAXLEN
    char format[18];                                       // Format string
    sprintf_s(format, sizeof(format), "%%-%ds Age:%%4d\n", MAXLEN);
    FILE *pfile = NULL;

    if (fopen_s(&pfile, filename, "rb")) {
        fprintf_s(stderr, "Unable to open %s. Verify it exists.\n", filename);
        return;
    }

    Record record;                                         // Stores a record
    printf_s("The folks recorded in the %s file are:\n", filename);

    while (read_record(&record, pfile))                    // As long as we have records
        printf_s(format, record.name, record.age);           // Output them

    printf_s("\n");                                        // Move to next line
    fclose(pfile);                                         // Close the file
}

// Modify existing records in the file
void update_file(void)
{
    FILE *pfile = NULL;

    if (fopen_s(&pfile, filename, "rb+")) {
        fprintf_s(stderr, " File open for updating records failed. Maybe file does not exist.\n");
        return;
    }

    Record record;                                         // Stores a record
    int index = find_record(&record, pfile);               // Find the record for a name

    if (index < 0) {                                       // If the record isn't there
        printf_s("Record not found.\n");                     // Output a message
        fclose(pfile);
        pfile = NULL;
        return;                                              // and we are done.
    }

    printf_s("%s is aged %d.\n", record.name, record.age);
    Record newrecord;                                      // Stores replacement record
    printf_s("You can now enter the new name and age for %s.\n", record.name);
    get_person(&newrecord);                                // Get the new record

    // Check if we can update in place
    if (strnlen_s(record.name, sizeof(record.name)) ==
        strnlen_s(newrecord.name, sizeof(record.name))) {
        // Name lengths are the same so we can update in place
        fseek(pfile,                                         // Move to start of old record
              -(long)(sizeof(size_t) + strnlen_s(record.name, sizeof(record.name)) + sizeof(record.age)),
              SEEK_CUR);
        write_record(&newrecord, pfile);                     // Write the new record
        fflush(pfile);                                       // Force the write
        fclose(pfile);                                       // Close the file
        pfile = NULL;
    } else
        duplicate_file(&newrecord, index, pfile);

    printf_s("File update complete.\n");
}

// Duplicate the existing file replacing the record to be updated
// The record to be replaced is index records from the start
void duplicate_file(const Record *pnewrecord, int index, FILE *pfile)
{
    // Create and open a new file
    char tempname[L_tmpnam_s];

    if (tmpnam_s(tempname, sizeof(tempname))) {
        fprintf_s(stderr, "Temporary file name creation failed.\n");
        exit(1);
    }

    FILE *ptempfile = NULL;

    if (fopen_s(&ptempfile, tempname, "wb+")) {
        fprintf_s(stderr, "Temporary file creation failed.\n");
        exit(1);
    }

    // Copy first index records from old file to new file
    rewind(pfile);                                         // Old file back to start
    Record record;                                         // Store for a record

    for (int i = 0 ; i < index ; ++i)
        write_record(read_record(&record, pfile), ptempfile);

    write_record(pnewrecord, ptempfile);                   // Write the new record
    read_record(&record, pfile);                           // Skip the old record

    // Copy the rest of the old file to the new file
    while (read_record(&record, pfile))
        write_record(&record, ptempfile);

    // close the files
    fclose(pfile);
    fclose(ptempfile);

    if (remove(filename)) {                                // Delete the old file
        fprintf_s(stderr, "Failed to remove the old file.\n");
        exit(1);
    }

    // Rename the new file same as original
    if (rename(tempname, filename)) {
        fprintf_s(stderr, "Renaming the file copy failed.\n");
        exit(1);
    }
}

// Find a record
// Returns the index number of the record
// or -1 if the record is not found.
int find_record(Record* precord, FILE* pfile)
{
    char name[MAXLEN];
    printf_s("Enter the name for the record you wish to find: ");
    get_name(name, MAXLEN);
    rewind(pfile);                                         // Make sure we are at the start
    int index = 0;                                         // Index of current record

    while (true) {
        if (!read_record(precord, pfile))                    // If NULL returned
            return -1;                                         // record not found

        if (!strcmp(name, precord->name))
            break;

        ++index;
    }

    return index;                                          // Return record index
}

