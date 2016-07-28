// Program 11.1 Exercising the horse
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

typedef struct Horse Horse;            // Define Horse as a type name

struct Horse {                         // Structure type definition
    int age;
    int height;
    char name[20];
    char father[20];
    char mother[20];
};

int main(void)
{
    Horse my_horse;                      // Structure variable declaration
    // Initialize  the structure variable from input data
    printf_s("Enter the name of the horse: ");
    scanf_s("%s", my_horse.name, sizeof(my_horse.name));     // Read the name
    printf_s("How old is %s? ", my_horse.name);
    scanf_s("%d", &my_horse.age);                            // Read the age
    printf_s("How high is %s ( in hands )? ", my_horse.name);
    scanf_s("%d", &my_horse.height);                         // Read the height
    printf_s("Who is %s's father? ", my_horse.name);
    scanf_s("%s", my_horse.father, sizeof(my_horse.father)); // Get pa's name
    printf_s("Who is %s's mother? ", my_horse.name);
    scanf_s("%s", my_horse.mother, sizeof(my_horse.mother)); // Get ma's name
    // Now tell them what we know
    printf_s("%s is %d years old, %d hands high,",
             my_horse.name, my_horse.age, my_horse.height);
    printf_s(" and has %s and %s as parents.\n", my_horse.father, my_horse.mother);
    return 0;
}
