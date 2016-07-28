// Ex4.5  A Modified Guessing Game
#include <stdio.h>
#include <stdlib.h>
#include <time.h>                           // For time() function
#include <ctype.h>                          // I added this for toupper()
#include <stdbool.h>                        // I added this for true

int main(void)
{
    int chosen = 0;                           // The lucky number
    int guess = 0;                            // Stores a guess
    const int max_tries = 3;                  // The maximum number of tries
    int limit = 20;                           // Upper limit for pseudo-random values
    char answer = 'N';
    srand(time(NULL));                        // Use clock value as starting seed
    printf("\nThis is a guessing game.\n");

    while (true) {
        chosen = 1 + rand() % limit;             // Random int 1 to limit
        printf("I have chosen a number between 1 and 20 which you must guess.\n");

        for (int count = max_tries ; count > 0 ; --count) {
            printf("\nYou have %d tr%s left.", count, count == 1 ? "y" : "ies");
            printf("\nEnter a guess: ");          // Prompt for a guess
            scanf("%d", &guess);                  // Read in a guess

            // Check for a correct guess
            if (guess == chosen) {
                printf("\nCongratulations. You guessed it!\n");
                break;                              // End the program
            } else if (guess < 1 || guess > 20)   // Check for an invalid guess
                printf("I said the number is between 1 and 20.\n ");
            else
                printf("Sorry, %d is wrong. My number is %s than that.\n",
                       guess, chosen > guess ? "greater" : "less");
        }

        printf("\nYou have had three tries and failed. The number was %ld\n", chosen);
        printf("Do you want to play again (Y or N)?");
        scanf(" %c", &answer);

        if (toupper(answer) != 'Y')
            break;
    }

    return 0;
}


