// Exercise 12.3 Writing names and phone numbers to a file.
/*
  This solution uses a PhoneRecord structure with the name and number stored in
  arrays with a fixed size. This allows the file operations to be very simple.
  You can just read or write a PhoneRecord structure since its size is fixed.

  If you wanted to allocate space for the name and number dynamically, then
  you would have to explicitly write the name and number as well as the
  PhoneRecord structure. You would also need to include the length of each
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

// Function prototypes
PhoneRecord read_phonerecord(void);          // Read a name and number from the keyboard
Name read_name(void);                        // Read a name from the keyboard
void list_records(char *filename);           // List all the records in the file
void show_record(const PhoneRecord *precord);// Output name and number from a phone record

int main(void)
{
    FILE *pFile = NULL;                        // Output file pointer
    char *filename = "C:\\records.bin";        // Name of the file to be written
    char answer = 'n';
    PhoneRecord record;
    printf_s("Do you want to enter some phone records(y or n)?: ");
    scanf_s(" %c", &answer, sizeof(answer));

    if (tolower(answer) == 'y') {
        if (fopen_s(&pFile, filename, "a+")) {
            printf_s("Error opening %s for writing. Program terminated.\n", filename);
            exit(1);
        }

        setvbuf(pFile, NULL, _IOFBF, 512);

        // Read an arbitrary number of phone records from the keyboard
        do {
            record = read_phonerecord();                 // Read the name and number
            fwrite(&record, sizeof record, 1, pFile);
            printf_s("Do you want to enter another(y or n)?: ");
            scanf_s(" %c", &answer, sizeof(answer));
        } while (tolower(answer) == 'y');

        fclose(pFile);                                    // Close file
        printf_s("\nFile write complete.");
    }

    printf_s("\nDo you want to list the records in the file(y or n)? ");
    scanf_s(" %c", &answer, sizeof(answer));

    if (tolower(answer) == 'y')
        list_records(filename);

    return 0;
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

// List all the records in the file
void list_records(char *filename)
{
    FILE *pFile;
    PhoneRecord record;
    bool file_empty = true;               // File empty flag

    if (fopen_s(&pFile, filename, "r")) {   // Open the file to read it
        printf_s("Error opening %s for reading. Program terminated.", filename);
        exit(1);
    }

    setvbuf(pFile, NULL, _IOFBF, 512);

    // List the file contents
    while (true) {
        fread(&record, sizeof record, 1, pFile);

        if (feof(pFile))
            break;

        file_empty = false;          // We got a record so set empty flag false
        show_record(&record);        // output the record
    }

    fclose(pFile);                 // Close the file

    // Check whether there were any records
    if (file_empty)
        printf_s("The file contains no records.\n");
    else
        printf_s("\n");
}

// Outputs the name and number from a phone record
void show_record(const PhoneRecord *precord)
{
    printf_s("\n%s %s   %s", precord->name.firstname, precord->name.secondname, precord->number);
}
