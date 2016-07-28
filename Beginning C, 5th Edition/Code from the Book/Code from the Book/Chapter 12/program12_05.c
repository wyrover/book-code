// Program 12.5 Investigating the family.
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_MAX 20

struct {
    char *filename;                              // Physical file name
    FILE *pfile;                                 // File pointer
} global = {.filename = "myfile.bin", .pfile = NULL};

// Structure types
typedef struct Date {                                 // Structure for a date
    int day;
    int month;
    int year;
} Date;

typedef struct Family {                               // Structure for family member
    Date dob;
    char name[NAME_MAX];
    char pa_name[NAME_MAX];
    char ma_name[NAME_MAX];
} Family;

// Function prototypes
bool get_person(Family *pfamily);                     // Input function for member details
void getname(char *name, size_t size);                // Read a name
void show_person_data(void);                          // Output function
void get_parent_dob(Family *pfamily);                 // Function to find DOB for pa & ma
void open_file(char *mode);                           // Open the file in the given mode
inline void close_file(void);                         // Close the file

int main(void)
{
    Family member;                                      // Stores a family structure
    open_file("ab");                                    // Open file to append to it

    while (get_person(&member))                         // As long as we have input
        fwrite(&member, sizeof member, 1, global.pfile);  //    write it away

    close_file();                                       // Close the file now its written
    show_person_data();                                 // Show what we can find out

    if (remove(global.filename))
        printf_s("Unable to delete %s.\n", global.filename);
    else
        printf_s("Deleted %s OK.\n", global.filename);

    return 0;
}

// Function to input data on Family members
bool get_person(Family *temp)
{
    static char more = '\0';                            // Test value for ending input
    printf_s("\nDo you want to enter details of %s person (Y or N)? ",
             more != '\0' ? "another" : "a");
    scanf_s(" %c", &more, sizeof(more));

    if (tolower(more) == 'n')
        return false;

    printf_s("Enter the name of the person: ");
    getname(temp->name, sizeof(temp->name));            // Get the person's name
    printf_s("Enter %s's date of birth (day month year): ", temp->name);
    scanf_s(" %d %d %d", &temp->dob.day, &temp->dob.month, &temp->dob.year);
    printf_s("Who is %s's father? ", temp->name);
    getname(temp->pa_name, sizeof(temp->pa_name));      // Get the father's name
    printf_s("Who is %s's mother? ", temp->name);
    getname(temp->ma_name, sizeof(temp->ma_name));      // Get the mother's name
    return true;
}

// Read a name from the keyboard
void getname(char *name, size_t size)
{
    fflush(stdin);                                      // Skip whitespace
    fgets(name, size, stdin);
    int len = strnlen_s(name, size);

    if (name[len - 1] == '\n')                          // If last char is newline
        name[len - 1] = '\0';                             // overwrite it
}

// Function to output data on people on file
void show_person_data(void)
{
    Family member;                                      // Structure to hold data from file
    open_file("rb");                                    // Open file for binary read

    // Read data on a person
    while (fread(&member, sizeof member, 1, global.pfile)) {
        printf_s("%s's father is %s, and mother is %s.\n",
                 member.name, member.pa_name, member.ma_name);
        get_parent_dob(&member);                          // Get parent data
    }

    close_file();                                       // Close the file
}

// Function to find parents' dates of birth.
void get_parent_dob(Family *pmember)
{
    Family relative;                                    // Stores a relative
    int num_found = 0;                                  // Count of relatives found
    fpos_t current;                                     // File position
    fgetpos(global.pfile, &current);                    // Save current position
    rewind(global.pfile);                               // Set file to the beginning

    // Get the stuff on a relative
    while (fread(&relative, sizeof(Family), 1, global.pfile)) {
        if (strcmp(pmember->pa_name, relative.name) == 0) {
            // We have found dear old dad */
            printf_s(" Pa was born on %d/%d/%d.", relative.dob.day, relative.dob.month, relative.dob.year);
            ++num_found;                                    // Increment parent count
        } else if (strcmp(pmember->ma_name, relative.name) == 0) {
            // We have found dear old ma
            printf_s(" Ma was born on %d/%d/%d.", relative.dob.day, relative.dob.month, relative.dob.year);
            ++num_found;                                    // Increment parent count
        }

        if (num_found == 2)                               // If we have both...
            break;                                          // ...we are done
    }

    if (!num_found)
        printf_s("  No info on parents available.");

    printf_s("\n");
    fsetpos(global.pfile, &current);                    // Restore file position file
}

// Open the file
void open_file(char *mode)
{
    if (global.pfile)
        close_file();

    if (fopen_s(&global.pfile, global.filename, mode)) {
        printf_s("Unable to open %s with mode %s.\n", global.filename, mode);
        exit(1);
    }

    setvbuf(global.pfile, NULL, _IOFBF, BUFSIZ);
}

// Close the file
inline void close_file(void)
{
    fclose(global.pfile);                               // Close the file
    global.pfile = NULL;                                // Set file pointer
}

