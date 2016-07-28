// Program 11.5 Daisy chaining the horses both ways
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Horse Horse;       // Define Horse as a type name

struct Horse {                    // Structure type definition
    int age;
    int height;
    char name[20];
    char father[20];
    char mother[20];
    Horse *next;                    // Pointer to next structure
    Horse *previous;                // Pointer to previous structure
};

int main(void)
{
    Horse *first = NULL;            // Pointer to first horse
    Horse *current = NULL;          // Pointer to current horse
    Horse *last = NULL;             // Pointer to previous horse
    char test = '\0';               // Test value for ending input

    for (; ;) {
        printf_s("Do you want to enter details of a%s horse (Y or N)? ",
                 first == NULL ? "nother " : "");
        scanf_s(" %c", &test, sizeof(test));

        if (tolower(test) == 'n')
            break;

        // Allocate memory for each new horse structure
        current = (Horse*) malloc(sizeof(Horse));

        if (first == NULL) {
            first = current;            // Set pointer to first horse
            current->previous = NULL;
        } else {
            last->next = current;       // Set next address for previous horse
            current->previous = last;   // Previous address for current horse
        }

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
        current->next = NULL;         // In case it's the last...
        last = current;               // ...save its address
    }

    // Now tell them what we know.
    printf_s("\n");

    while (current != NULL) {       // Output horse data in reverse order
        printf_s("%s is %d years old, %d hands high,",
                 current->name, current->age, current->height);
        printf_s(" and has %s and %s as parents.\n", current->father,
                 current->mother);
        last = current;               // Save pointer to enable memory to be freed
        current = current->previous;  // current points to previous in list
        free(last);                   // Free memory for the horse we output
        last = NULL;
    }

    first = NULL;
    return 0;
}

