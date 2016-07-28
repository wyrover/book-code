// Program 11.4   Daisy chaining the horses
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Horse Horse;            // Define Horse as a type name

struct Horse {                         // Structure type definition
    int age;
    int height;
    char name[20];
    char father[20];
    char mother[20];
    Horse *next;                         // Pointer to next Horse structure
};

int main(void)
{
    Horse *first = NULL;                 // Pointer to first horse
    Horse *current = NULL;               // Pointer to current horse
    Horse *previous = NULL;              // Pointer to previous horse
    char test = '\0';                    // Test value for ending input

    for (; ;) {
        printf_s("Do you want to enter details of a%s horse (Y or N)? ",
                 first != NULL ? "nother " : "");
        scanf_s(" %c", &test, sizeof(test));

        if (tolower(test) == 'n')
            break;

        // Allocate memory for a Horse structure
        current = (Horse*) malloc(sizeof(Horse));

        if (first == NULL)                 // If there's no 1st Horse...
            first = current;                 // ...set this as 1st Horse

        if (previous != NULL)              // If there was a previous...
            previous->next = current;        // ...set its next to this one

        printf_s("Enter the name of the horse: ");
        scanf_s("%s", current->name, sizeof(current->name));
        printf_s("How old is %s? ", current->name);
        scanf_s("%d", &current->age);
        printf_s("How high is %s ( in hands )? ", current -> name);
        scanf_s("%d", &current->height);
        printf_s("Who is %s's father? ", current->name);
        scanf_s("%s", current->father, sizeof(current->father));
        printf_s("Who is %s's mother? ", current->name);
        scanf_s("%s", current->mother, sizeof(current->mother));
        current->next = NULL;             // In case it's the last...
        previous = current;               // ...save its address
    }

    // Now tell them what we know...
    printf_s("\n");
    current = first;                    // Start at the beginning

    while (current != NULL) {           // As long as we have a valid pointer
        // Output the data
        printf_s("%s is %d years old, %d hands high,",
                 current->name, current->age, current->height);
        printf_s(" and has %s and %s as parents.\n", current->father,
                 current->mother);
        previous = current;    // Save the pointer so we can free memory
        current = current->next;          // Get the pointer to the next
        free(previous);                   // Free memory for the old one
        previous = NULL;
    }

    first = NULL;
    return 0;
}

