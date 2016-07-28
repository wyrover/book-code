// Exercise 6.4 Looking for palindromes
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 500                             // Maximum sentence length

int main(void)
{
    char sentence[MAX_LEN];                        // Stores the sentence to be tested
    char sentence_chars[MAX_LEN];                  // Stores the sentence without punctuation and spaces
    size_t j = 0;                                  // Index to character position
    size_t length = 0;                             // Length of a string
    printf("Enter a sentence to be tested:\n");
    gets_s(sentence, MAX_LEN);

    // Copy only letters as lowercase
    for (size_t i = 0 ; i < strnlen_s(sentence, MAX_LEN) ; ++i)
        if (isalpha(sentence[i]))
            sentence_chars[j++] = tolower(sentence[i]);

    sentence_chars[j] = '\0';                      // Append string terminator
    length = strnlen_s(sentence_chars, MAX_LEN);   // Get the string length
    // Compare matching characters in the string
    // If any pair are not the same, then it's not a palindrome
    bool isPalindrome = true;

    for (size_t i = 0 ; i < length / 2 ; ++i) {
        if (sentence_chars[i] != sentence_chars[length - 1 - i]) {
            isPalindrome = false;
            break;
        }
    }

    printf("\n The sentence you entered is%sa palindrome.\n", isPalindrome ? " " : " not ");
    return 0;
}

