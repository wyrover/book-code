// Exercise 8.4 A function to return the number of words in a string passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LENGTH_INCREMENT  20
#define MAX_WORD_LENGTH   30

int countwords(char str[]);                                    // Function to count words in a string
int segment_string(char str[], char *words[]);                 // Function to segment a string
int count_letters(char str[]);                                 // Function to count letters in a word


int main(void)
{
    char* pStr = NULL;             // Points to memory that stores the string
    char* pTemp = NULL;            // Temporary pointer
    char** words = NULL;           // Pointer to an array of words
    int length = 0;                // Current string length
    int max_length = 0;            // Current maximum string length
    int wordcount = 0;             // Count of words in string
    // Read the string
    printf("Enter a string:\n");

    do {
        // If there is not enough memory, allocate some more
        if (length >= max_length) {
            max_length += LENGTH_INCREMENT;

            if (!(pTemp = realloc(pStr, max_length))) {
                printf("Memory allocation for string failed.\n");
                exit(1);
            }

            pStr = pTemp;
        }
    } while ((pStr[length++] = getchar()) != '\n');

    pStr[--length] = '\0';         // Append string terminator
    wordcount = countwords(pStr);  // Get the word count
    // Now allocate memory to hold the words
    words = (char**)malloc(wordcount * sizeof(char*));

    for (int i = 0 ; i < wordcount ; ++i)
        words[i] = (char*)malloc(MAX_WORD_LENGTH);

    segment_string(pStr, words);   // Segment the string into words

    // Sort the words in order of length */
    for (size_t i = 0 ; i < wordcount - 1 ; ++i) {
        for (size_t j = i ; j < wordcount ; ++j) {
            if (count_letters(words[i]) > count_letters(words[j])) {
                pTemp = words[i];
                words[i] = words[j];
                words[j] = pTemp;
            }
        }
    }

    // Output the words and free the memory
    printf("The words in the string in order of length are:\n");

    for (size_t i = 0 ; i < wordcount ; ++i) {
        printf("%s\n", words[i]);
        free(words[i]);
    }

    free(words);      // Free the memory for the pointers
    return 0;
}

// Function to count words in a string
int countwords(char str[])
{
    int count = 0;
    size_t i = 0;

    while (str[i] != '\0') {
        if (!isalpha(str[i])) {
            i++;
            continue;
        }

        ++count;

        while (isalpha(str[++i]))
            ;
    }

    return count;
}

// Function to segment a string into words and return he number of words
int segment_string(char str[], char *words[])
{
    int count = 0;
    size_t i = 0;
    size_t j = 0;

    while (str[i] != '\0') {
        if (!isalpha(str[i])) {
            i++;
            continue;
        }

        j = 0;

        while (isalpha(str[i])) {
            words[count][j++] = str[i++];
        }

        words[count++][j] = '\0';
    }

    return count;
}

// Function to count letters in a word
int count_letters(char str[])
{
    int count = 0;
    size_t i = 0;

    while (str[i] != '\0') {
        if (isalpha(str[i++]))
            ++count;
    }

    return count;
}
