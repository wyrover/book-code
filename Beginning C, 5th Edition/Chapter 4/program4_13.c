// Program 4.13 Simple Simon
#include <stdio.h>                   // For input and output 
#include <ctype.h>                   // For toupper() function
#include <stdbool.h>                 // For bool, true, false
#include <stdlib.h>                  // For rand() and srand()
#include <time.h>                    // For time() function

int main(void)
{
    char another_game = 'Y';             // Records if another game is to be played
    const unsigned int DELAY = 1;        // Display period in seconds
    bool correct = true;                 // true for correct sequence, false otherwise
    unsigned int tries = 0;              // Number of successful entries for sequence length
    unsigned int digits = 0;             // Number of digits in a sequence
    time_t seed = 0;                     // Seed value for random number sequence
    unsigned int number = 0;             // Stores an input digit
    time_t now = 0;                      // Stores current time
    time_t start_time = 0;               // Game start time in clock ticks
    unsigned int score = 0;              // Game score
    unsigned int total_digits = 0;       // Total of digits entered in a game
    unsigned int game_time = 0;          // Game time in seconds
    // Describe how the game is played
    printf("\nTo play Simple Simon, ");
    printf("watch the screen for a sequence of digits.");
    printf("\nWatch carefully, as the digits are only displayed"
           " for %u second%s ", DELAY, DELAY > 1 ? "s!" : "!");
    printf("\nThe computer will remove them, and then prompt you ");
    printf("to enter the same sequence.");
    printf("\nWhen you do, you must put spaces between the digits.\n");
    printf("\nGood Luck!\nPress Enter to play\n");
    scanf("%c", &another_game);

    // Game loop - one outer loop iteration is a complete game
    do {
        // Initialize game
        correct = true;                    // Indicates correct sequence entered
        tries = 0;                         // Initialize count of successful tries
        digits = 2;                        // Initial length of digit sequence
        start_time = clock();              // Record time at start of game

        // Inner loop continues as long as sequences are entered correctly
        while (correct) {
            ++tries;                         // A new attempt
            now = clock();                   // record start time for sequence
            // Generate a sequence of digits and display them
            srand(time(&seed));              // Initialize the random sequence

            for (unsigned int i = 1 ; i <= digits ; ++i)
                printf("%u ", rand() % 10);    // Output a random digit

            for (; clock() - now < DELAY * CLOCKS_PER_SEC;); // Wait DELAY seconds

            // Now overwrite the digit sequence
            printf("\r");                    // Go to beginning of the line

            for (unsigned int i = 1 ; i <= digits ; ++i)
                printf("  ");                  // Output two spaces

            if (tries == 1)                  // Only output message for 1st try
                printf("\nNow you enter the sequence  - don't forget"
                       " the spaces\n");
            else
                printf("\r");                  // Back to the beginning of the line

            srand(seed);                     // Reinitialize the random sequence

            for (unsigned int i = 1 ; i <= digits ; ++i)
                // Read the input sequence & check against the original
            {
                scanf("%u", &number);          // Read a digit

                if (number != rand() % 10) {   // Compare with generated digit
                    correct = false;             // Incorrect entry
                    break;                       // No need to check further...
                }
            }

            // On every third successful try, increase the sequence length
            if (correct && ((tries % 3) == 0))
                ++digits;

            printf("%s\n", correct ? "Correct!" : "Wrong!");
        }

        // Calculate and output the game score
        score = 10 * (digits - ((tries % 3) == 1));              // Points for sequence length
        total_digits = digits * (((tries % 3) == 0) ? 3 : tries % 3);

        if (digits > 2)
            total_digits += 3 * ((digits - 1) * (digits - 2) / 2 - 1);

        game_time = (clock() - start_time) / CLOCKS_PER_SEC - tries * DELAY;

        if (total_digits > game_time)
            score += 10 * (game_time - total_digits);              // Add points for speed

        printf("\n\nGame time was %u seconds. Your score is %u", game_time, score);
        fflush(stdin);                     // Clear the input buffer
        // Check if new game required
        printf("\nDo you want to play again (y/n)? ");
        scanf("%c", &another_game);
    } while (toupper(another_game) == 'Y');

    return 0;
}

