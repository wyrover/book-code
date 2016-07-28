// Program 2.4 Simple calculations
#include <stdio.h>

int main(void)
{
    int total_pets;
    int cats;
    int dogs;
    int ponies;
    int others;
    // Set the number of each kind of pet
    cats = 2;
    dogs = 1;
    ponies = 1;
    others = 46;
    // Calculate the total number of pets
    total_pets = cats + dogs + ponies + others;
    printf("We have %d pets in total\n", total_pets);   // Output the result
    return 0;
}
