// Exercise 11.4 Using a structure to record the count of occurrences of a word.
/*
  This program defines a structure that stores a word and the count of its occurrences.
  The structures are stored as they are created in a linked list. If you wanted the
  output in alphabetical order, you could insert the structures in the list to ensure
  that the list was ordered. Alternatively you could sort the list.
*/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH    31
#define MAX_TEXT_LENGTH 10000

// Structure defining a count of the occurrences of a given word
typedef struct WordCounter {
    char *word;
    int word_count;
    struct WordCounter *pNext;                        // Pointer to the next word counter in the list
} WordCounter;

// Function prototypes
void addWord(char *pWord);                          // Adds a new word to the list or updates existing word
void show(const WordCounter *pWordcounter);         // Outputs a word and its count of occurrences
WordCounter* createWordCounter(char *word);         // Creates a new WordCounter structure

// Global variables
WordCounter *pStart = NULL;                         // Pointer to first word counter in the list

int main(void)
{
    char text[MAX_TEXT_LENGTH];                       // Stores input text
    WordCounter *pCounter = NULL;                     // Pointer to a word counter
    // Read the text from the keyboard
    printf_s("Enter the text:\n");
    gets_s(text, MAX_TEXT_LENGTH);
    // Extract the words from the text
    size_t text_len = sizeof(text);
    char *ptr = NULL;
    char separators[] = { ' ' , ',', ':' , '\"', '?' , '!' , '.'};
    char *pWord = strtok_s(text, &text_len, separators, &ptr);  // Find 1st word

    while (pWord) {
        // While there are still words...
        addWord(pWord);                                           // Add thew word to the list
        pWord = strtok_s(NULL, &text_len, separators, &ptr);      // .. and find the next one
    }

    // List the words and their counts
    pCounter = pStart;

    while (pCounter) {
        show(pCounter);
        pCounter = pCounter->pNext;
    }

    printf_s("\n");
    // Free the memory that we allocated
    pCounter = pStart;

    while (pCounter) {
        free(pCounter->word);                           // Free space for the word
        pStart = pCounter;                              // Save address of current
        pCounter = pCounter->pNext;                     // Move to next counter
        free(pStart);                                   // Free space for current
    }

    return 0;
}

// Output a word and its count
void show(const WordCounter *pWordcounter)
{
    // output the word left-justified in a fixed field width followed by the count
    printf_s("\n%-30s   %5d", pWordcounter->word, pWordcounter->word_count);
}

void addWord(char *word)
{
    WordCounter *pCounter = NULL;
    WordCounter *pLast = NULL;

    if (!pStart) {
        pStart = createWordCounter(word);
        return;
    }

    // If the word is in the list, increment its count
    pCounter = pStart;

    while (pCounter) {
        if (strcmp(word, pCounter->word) == 0) {
            ++pCounter->word_count;
            return;
        }

        pLast = pCounter;                               // Save address of last in case we need it
        pCounter = pCounter->pNext;                     // Move to next in the list
    }

    // If we get to here it's not in the list - so add it
    pLast->pNext = createWordCounter(word);
}

// Create and returns a new WordCounter object for the argument
WordCounter* createWordCounter(char *word)
{
    WordCounter *pCounter = NULL;
    pCounter = (WordCounter*)malloc(sizeof(WordCounter));
    pCounter->word = (char*)malloc(strlen(word) + 1);
    strcpy(pCounter->word, word);
    pCounter->word_count = 1;
    pCounter->pNext = NULL;
    return pCounter;
}
