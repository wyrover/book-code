// Exercise 11.2 Using a structure representing a person's name.
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FIRST_NAME_LEN  31
#define SECOND_NAME_LEN 51
#define NUMBER_LEN      16
#define MAX_NUMBERS     50


// Structure defining a name
typedef struct Name {
    char firstname[FIRST_NAME_LEN];
    char secondname[SECOND_NAME_LEN];
} Name;

// Structure defining a phone record
typedef struct PhoneRecord {
    struct Name name;
    char number[NUMBER_LEN];
} PhoneRecord;

// Function prototypes
size_t get_records(PhoneRecord records[], size_t size);                                  // Read phone records
void search_records(PhoneRecord records[], size_t count);                                // Search the phone records
int find_record(PhoneRecord records[], size_t start, size_t count, const Name *pName);   // Find the record for a name
void show(const PhoneRecord *pRecord);                                                     // Output a phone record
bool has_name(const PhoneRecord *pRecord, const Name *pName);                            // Test for a name
Name read_name();                                                                        // Read a name from the keyboard

int main(void)
{
    char answer = 'n';
    PhoneRecord records[MAX_NUMBERS];                                                      // Array of phone records
//  Name aName;                                                                          // Stores a name
    size_t count = get_records(records, MAX_NUMBERS);                                      // Number of phone records
    printf_s("\nDo you want to search the phone records (y or n)? ");
    scanf_s(" %c" , &answer, sizeof(answer));

    if (tolower(answer) == 'y')
        search_records(records, count);

    printf_s("\nThe records we have are:\n");

    for (size_t i = 0 ; i < count ; ++i)
        show(records[i]);

    printf_s("\n");
    return 0;
}

// Function to read an arbitrary number of phone records from the keyboard
size_t get_records(PhoneRecord records[], size_t size)
{
    size_t count = 0;
    char answer = 'y';

    do {
        records[count].name = read_name();                                        // Read the name
        printf_s("Enter the number for this name: ");
        scanf_s(" %[ 0123456789]", records[count++].number, (rsize_t)NUMBER_LEN); // Read the number - including spaces
        printf_s("Do you want to enter another(y or n)?: ");
        scanf_s(" %c", &answer, sizeof(answer));
    } while (count <= size && tolower(answer) == 'y');

    return count;
}

// Function to output a record
void show(const PhoneRecord *pRecord)
{
    printf_s("\n%s %s   %s", pRecord->name.firstname, pRecord->name.secondname, pRecord->number);
}

// Function to test whether the name is the same as in a record
bool has_name(const PhoneRecord *pRecord, const Name *pName)
{
    return (strcmp(pName->firstname, pRecord->name.firstname) == 0 &&
            strcmp(pName->secondname, pRecord->name.secondname) == 0);
}

// Function to read a name and store it in a Name structure
Name read_name(void)
{
    Name name;
    printf_s("Enter a first name: ");
    scanf_s(" %s", name.firstname, (rsize_t)FIRST_NAME_LEN);
    printf_s("Enter a second name: ");
    scanf_s(" %s", name.secondname, (rsize_t)SECOND_NAME_LEN);
    return name;
}

// Function to find the record for a name in records starting at index start.
// count is the number of elements in records.
// The index to the record is returned or -1 if it doesn't exist.
int find_record(PhoneRecord records[], size_t start, size_t count, const Name *pName)
{
    for (size_t i = start ; i < count ; ++i) {
        if (has_name(&records[i], pName))                             // Test for the name
            return i;
    }

    return -1;
}

// Search the array of phone records for a number
void search_records(PhoneRecord records[], size_t count)
{
    Name name;
    char answer = 'n';

    do {
        name = read_name();
        int index = 0;
        bool first = true;

        while ((index = find_record(records, index, count, &name)) >= 0) {
            if (first) {
                printf_s("The numbers for %s %s are:\n", name.firstname, name.secondname);
                first = false;
            }

            printf_s("%s\n", records[index++].number);

            if (index >= count)
                break;
        }

        if (first)
            printf_s("No numbers found for %s %s.\n", name.firstname, name.secondname);

        printf_s("\nDo you want to search for another name (y or n)? ");
        scanf_s(" %c" , &answer, sizeof(answer));
    } while (tolower(answer) == 'y');
}
