// Exercise 12.4 Writing names and phone numbers to a file. */
/*
  This solution uses a PhoneRecord structure with the name and number stored in
  arrays with a fixed size. This allows the file operations to be very simple.
  You can just read or write a PhoneRecord structure since its size is fixed.

  If you wanted to allocate space for the name and number dynamically, then
  you would have to explicitly write the name and number strings as well as the
  PhoneRecord structure to the file. You would also need to include the length of each
  data item in the file so you knew how much to read back.
*/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define FIRST_NAME_LENGTH  31
#define SECOND_NAME_LENGTH 51
#define NUMBER_LENGTH      21


// Structure defining a name
typedef struct NName {
    char firstname[FIRST_NAME_LENGTH];
    char secondname[SECOND_NAME_LENGTH];
} Name;

// Structure defining a phone record
typedef struct PPhoneRecord {
    Name name;
    char number[NUMBER_LENGTH];
} PhoneRecord;


// Function prototypes */
PhoneRecord read_phonerecord(void);                   // Read a name and number from the keyboard
Name read_name(void);                                 // Read a name from the keyboard
void list_records(char *filename);                    // List all the records in the file
void show_record(const PhoneRecord *precord);         // Output name and number from a phone record
void find_numbers(Name *pname, char *filename);       // Find numbers corresponding to a given name
void add_record(char *filename);                      // Add a new name and number to the file
void delete_records(Name *pname, char *filename);     // Delete records for a given name
void show_operations(void);                           // Displays operations supported by the program
int equals(Name *name1, Name *name2);                 // Compare two names for equality


int main(void)
{
    char *filename = "C:\\records.bin";                 // Name of the file holding the records
    char answer = 'n';                                  // Stores input responses
    show_operations();                                  // Display the available operations

    while (true) {
        printf_s("\nEnter a letter to select an operation: ");
        scanf_s(" %c", &answer, sizeof(answer));
        Name name;

        switch (toupper(answer)) {
        case 'A':                                       // Add a new name and number record
            add_record(filename);
            break;

        case 'D':                                       // Delete records for a given name
            name = read_name();
            delete_records(&name, filename);
            break;

        case 'F':                                       // Find the numbers for a given name
            name = read_name();
            find_numbers(&name, filename);
            break;

        case 'L':                                       // List all the name/number records
            list_records(filename);
            break;

        case 'Q':                                       // Quit the program
            return 0;

        default:
            printf_s("Invalid selection try again.\n");
            show_operations();
            break;
        }
    }
}

// Reads a name and number from the keyboard and creates a PhoneRecord structure
PhoneRecord read_phonerecord(void)
{
    PhoneRecord record;
    record.name = read_name();
    printf_s("Enter the number: ");
    scanf_s(" %[ 0123456789]", record.number, sizeof(record.number)); // Read the number - including spaces
    return record;
}

// Outputs the name and number from a phone record
void show_record(const PhoneRecord *precord)
{
    printf_s("\n%s %s   %s", precord->name.firstname, precord->name.secondname, precord->number);
}

// Add a new name and number
void add_record(char *filename)
{
    FILE *pFile = NULL;
    PhoneRecord record;

    if (fopen_s(&pFile, filename, "a+")) {       // Open/create file to be written in append mode
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    record = read_phonerecord();                 // Read the name and number
    fwrite(&record, sizeof record, 1, pFile);
    fclose(pFile);                               // Close file
    printf_s("New record added.\n");
}

// Read a name from the keyboard and create a Name structure for it
Name read_name(void)
{
    Name name;
    printf_s("Enter a first name: ");
    scanf_s(" %s", name.firstname, sizeof(name.firstname));
    printf_s("Enter a second name: ");
    scanf_s(" %s", name.secondname, sizeof(name.secondname));
    return name;
}

// Delete records for a given name
/* To delete one or more records we can copy
   the contents of the existing file to a new
   file, omitting the records that are to be
   deleted. We can then delete the old file and
   rename the new file to have the old file
   name.
*/
void delete_records(Name *pname, char *filename)
{
    FILE *pFile = NULL;
    FILE *pNewFile = NULL;
    char *pnewfilename = NULL;
    char answer = 'n';
    PhoneRecord record;

    if (fopen_s(&pFile, filename, "r")) {                // Open current file to read it
        printf_s("Error opening %s for reading. Program terminated.\n", filename);
        exit(1);
    }

    pnewfilename = tmpnam(NULL);                       // Create temporary file name

    if (fopen_s(&pNewFile, pnewfilename, "w")) {       // Open temporary file to write it
        printf_s("Error opening %s for writing. Program terminated.\n", pnewfilename);
        fclose(pFile);
        exit(1);
    }

    // Copy existing file contents to temporary file, omitting deleted records
    while (true) {
        fread(&record, sizeof record, 1, pFile);         // Read a record

        if (feof(pFile))                                 // if it's end of file...
            break;                                         // ...quit copy loop

        if (equals(pname, &(record.name))) {             // Is the record this name?
            printf_s("Found a record:\n");                 // Ys, so it's a delete candidate
            show_record(&record);
            printf_s("\nDo you really want to delete it (y or n)? ");
            scanf_s(" %c", &answer, sizeof(answer));

            if (tolower(answer) == 'y')                    // If it's to be deleted
                continue;                                    // Skip the copying
        }

        fwrite(&record, sizeof record, 1, pNewFile);     // copy current record
    }

    fclose(pFile);
    fclose(pNewFile);

    if (fopen_s(&pNewFile, pnewfilename, "r")) {       // Open temporary file to read it
        printf_s("Error opening %s for reading. Program terminated.", pnewfilename);
        exit(1);
    }

    if (fopen_s(&pFile, filename, "w")) {              // Open original file to write it
        printf_s("Error opening %s for writing. Program terminated.\n", filename);
        exit(1);
    }

    // Copy contents of new temporary file back to old file
    // This overwrites the original contents because the mode is "w"
    while (true) {
        fread(&record, sizeof record, 1, pNewFile);      // Read temporary file

        if (feof(pNewFile))                              // If we read EOF
            break;                                         // We are done

        fwrite(&record, sizeof record, 1, pFile);       // Write record to original file
    }

    fclose(pFile);                                     // Close the original file
    fclose(pNewFile);                                  // Close the temporary file
    remove(pnewfilename);                              // Delete the temporary file
    printf_s("Delete complete.");
}

// List all the records in the file
void list_records(char *filename)
{
    FILE *pFile;
    PhoneRecord record;
    bool file_empty = true;                            // File empty flag

    if (fopen_s(&pFile, filename, "r")) {              // Open the file to read it
        printf_s("Error opening %s for reading. Program terminated.", filename);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);

    // List the file contents
    while (true) {
        fread(&record, sizeof record, 1, pFile);

        if (feof(pFile))
            break;

        file_empty = false;                              // We got a record so set empty flag false
        show_record(&record);                            // output the record
    }

    fclose(pFile);                                     // Close the file

    // Check whether there were any records
    if (file_empty)
        printf_s("The file contains no records.\n");
    else
        printf_s("\n");
}

// Displays the operations that are supported by the program
void show_operations(void)
{
    printf_s("The operations available are:\n"
             "A: Add a new name and number entry.\n"
             "D: Delete all existing entries for a name.\n"
             "F: Find the number(s) for a given name.\n"
             "L: List all the entries in the file.\n"
             "Q: Quit the program.\n");
}

// Find numbers corresponding to a given name
void find_numbers(Name *pname, char *filename)
{
    FILE *pFile = NULL;
    PhoneRecord record;
    bool name_found = false;                           // Name found flag

    if (fopen_s(&pFile, filename, "r")) {              // Open the file to read it
        printf_s("Error opening %s for reading. Program terminated.", filename);
        exit(1);
    }

    // Search the records read from the file
    while (true) {
        fread(&record, sizeof record, 1, pFile);         // Read a record

        if (feof(pFile))
            break;

        if (equals(pname, &(record.name))) {              // Is it the name requested?
            if (!name_found) {                              // Is this the first time we found it?
                name_found = true;                            // Yes so set flag to true
                printf_s("The numbers for this name are:");   // Output initial message
            }

            printf_s("\n%s", record.number);                // Output the number
        }
    }

    fclose(pFile);                                     // Close the file

    // Check for name not found
    if (!name_found)
        printf_s("The name was not found.\n");
    else
        printf_s("\n");
}

// Compare two names for equality
int equals(Name *pname1, Name *pname2)
{
    return (strcmp(pname1->firstname, pname2->firstname) == 0) && (strcmp(pname1->secondname, pname2->secondname) == 0);
}
