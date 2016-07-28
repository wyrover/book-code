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
    Horse *phorses[50];         // Array of pointers to structure
    int hcount = 0;            // Count of the number of horses
    char test = '\0';          // Test value for ending input

    for (hcount = 0 ; hcount < sizeof(phorses) / sizeof(Horse*) ; ++hcount) {
        printf_s("Do you want to enter details of a%s horse (Y or N)? ",
                 hcount ? "nother" : "");
        scanf_s(" %c", &test, sizeof(test));

        if (tolower(test) == 'n')
            break;

        // allocate memory to hold a structure
        phorses[hcount] = (Horse*) malloc(sizeof(Horse));
        printf_s("Enter the name of the horse: ");
        scanf_s("%s", phorses[hcount]->name, sizeof(phorses[hcount]->name));
        printf_s("How old is %s? ", phorses[hcount]->name);
        scanf_s("%d", &phorses[hcount]->age);
        printf_s("How high is %s ( in hands )? ", phorses[hcount]->name);
        scanf_s("%d", &phorses[hcount]->height);
        printf_s("Who is %s's father? ", phorses[hcount]->name);
        scanf_s("%s", phorses[hcount]->father, sizeof(phorses[hcount]->father));
        printf_s("Who is %s's mother? ", phorses[hcount]->name);
        scanf_s("%s", phorses[hcount]->mother, sizeof(phorses[hcount]->mother));
    }

    // Now tell them what we know.
    printf_s("\n");

    for (int i = 0 ; i < hcount ; ++i) {
        printf_s("%s is %d years old, %d hands high,",
                 phorses[i]->name, phorses[i]->age, phorses[i]->height);
        printf_s(" and has %s and %s as parents.\n", phorses[i]->father,
                 phorses[i]->mother);
        free(phorses[i]);
    }

    return 0;
}

