// Program 10.1        Exercising formatted input
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

#define SIZE  20               // Max characters in a word 
void try_input(char *prompt, char *format);  // Input test function

int main(void)
{
    try_input("Enter as input: -2.35 15 25 ready2go\n",
              "%f %d %d %[abcdefghijklmnopqrstuvwxyz] %*1d %s%n");
    try_input("\nEnter the same input again: ",
              "%4f %4d %d %*d %[abcdefghijklmnopqrstuvwxyz] %*1d %[^o]%n");
    try_input("\nEnter as input: -2.3A 15 25 ready2go\n",
              "%4f %4d %d %*d %[abcdefghijklmnopqrstuvwxyz] %*1d %[^o]%n");
    return 0;
}

void try_input(char* prompt, char *format)
{
    int value_count = 0;       // Count of input values read
    float fp1 = 0.0;           // Floating-point value read
    int i = 0;                 // First integer read
    int j = 0;                 // Second integer read
    char word1[SIZE] = " ";    // First string read
    char word2[SIZE] = " ";    // Second string read
    int byte_count = 0;        // Count of input bytes read
    printf(prompt);
    value_count = scanf_s(format, &fp1, &i , &j, word1, sizeof(word1), word2, sizeof(word2), &byte_count);
    fflush(stdin);             // Clear the input buffer
    printf("The input format string for scanf_s() is:\n     \"%s\"\n", format);
    printf("Count of bytes read = %d\n", byte_count);
    printf("Count of values read = %d\n", value_count);
    printf("fp1 = %f   i = %d   j = %d\n", fp1, i, j);
    printf("word1 = %s   word2 = %s\n", word1, word2);
}
