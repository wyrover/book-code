// Program 11.3 Pointing out the horses
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>          // For malloc() 

typedef struct Horse Horse;  // Define Horse as a type name

struct Horse {               // Structure type definition
    int age;
    int height;
    char name[20];
    char father[20];
    char mother[20];
};

int main(void)
{
    Horse *phorse[50];         // Array of pointers to structure
    int hcount = 0;            // Count of the number of horses
    char test = '\0';          // Test value for ending input

    for (hcount = 0 ; hcount < 50 ; ++hcount) {
        printf_s("Do you want to enter details of a%s horse (Y or N)? ",
                 hcount ? "nother " : "");
        scanf_s(" %c", &test, sizeof(test));

        if (tolower(test) == 'n')
            break;

        // allocate memory to hold a structure
        phorse[hcount] = (Horse*) malloc(sizeof(Horse));
        printf_s("Enter the name of the horse: ");
        scanf_s("%s", phorse[hcount]->name, sizeof(phorse[hcount]->name));
        printf_s("How old is %s? ", phorse[hcount]->name);
        scanf_s("%d", &phorse[hcount]->age);
        printf_s("How high is %s ( in hands )? ", phorse[hcount]->name);
        scanf_s("%d", &phorse[hcount]->height);
        printf_s("Who is %s's father? ", phorse[hcount]->name);
        scanf_s("%s", phorse[hcount]->father, sizeof(phorse[hcount]->father));
        printf_s("Who is %s's mother? ", phorse[hcount]->name);
        scanf_s("%s", phorse[hcount]->mother, sizeof(phorse[hcount]->mother));
    }

    // Now tell them what we know.
    printf_s("\n");

    for (int i = 0 ; i < hcount ; ++i) {
        printf_s("%s is %d years old, %d hands high,",
                 phorse[i]->name, phorse[i]->age, phorse[i]->height);
        printf_s(" and has %s and %s as parents.\n", phorse[i]->father,
                 phorse[i]->mother);
        free(phorse[i]);
    }

    return 0;
}

