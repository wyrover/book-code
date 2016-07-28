// Exercise 10.1 Reading types of strings from the keyboard
/*
type 1: a sequence of lowercase letters followed by a digit. e.g. number1
type 2: two words that both begin with a capital letter and have a hyphen between them. e.g. Seven-Up
type 3: a decimal value. e.g. 7.35
type 4: a sequence of upper or lower case letters and spaces. e.g. Oliver Hardy
type 5: a sequence of any characters except spaces and digits that does not
        start with a letter. e.g. floating-point

The simple approach is to use the fact that each string after the first starts with
a character that does not appear in the previous string. Thus you can specify the characters for
each string as anything that is not the first character in the string that follows.
This approach would allow illegal characters in the string to be read.
e.g. the first string could be read as any lowercase letter or digit.

To positively ensure the strings are in the form specified is much more work, as the
code here shows.
If you wanted to allow erroneous data to be re-entered, it would be easiest if you
require each input to be on a separate line.
*/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define LEN 80
int main(void)
{
    const rsize_t LEN = 80;
    char word1[LEN];
    char word2[LEN];
    char word3[LEN];
    char word4[LEN];
    char word5[LEN];
    char temp[LEN];
    int k = 0;
    printf_s("Enter the five strings:\n");
    // Read first string, first the lowercase letters, then the digit
    k = scanf_s(" %[abcdefghijklmnopqrstuvwxyz]%1[0123456789]", word1, LEN, temp, LEN);

    if (k < 2) {
        printf_s("Incorrect input for first string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    strcat_s(word1, LEN, temp);
    // Read second string, first the uppercase letter, then the following
    // letters, then the hyphen, then the uppercase letter, finally the
    // following letters.
    k = scanf_s(" %1[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", word2, LEN);

    if (k < 1) {
        printf("Incorrect input for second string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    scanf_s("%[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]", temp, LEN);
    strcat_s(word2, LEN, temp);
    k = scanf_s("%1[-]", temp, LEN);

    if (k < 1) {
        printf_s("Incorrect input for second string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    k = scanf_s(" %1[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", temp, LEN);

    if (k < 1) {
        printf_s("Incorrect input for second string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    strcat_s(word2, LEN, temp);

    if (EOF == scanf_s("%[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]", temp, LEN)) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    strcat_s(word2, LEN, temp);
    // Read the third string. I cheat a bit because I read this assuming it is a well-formed
    // decimal string. To verify that is really is, you could read it as a decimal
    // value and then convert it to a string.
    k = scanf_s(" %[0123456789.-]", word3, LEN);

    if (k < 1) {
        printf_s("Incorrect input for third string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    // Read the fourth string.
    scanf_s("%[ abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]", word4, LEN);

    if (k < 1) {
        printf_s("Incorrect input for fourth string - program terminated.\n");
        exit(1);
    } else if (k == EOF) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    // Read the fifth string. You must exclude newline, otherwise it will be
    // read as part of the string.
    if (EOF == scanf_s(" %[^ 0123456789\n]", word5, LEN)) {
        printf_s("Input error - program terminated.\n");
        exit(1);
    }

    printf_s("The input strings read are: \n%s  %s  %s  %s  %s\n",
             word1, word2, word3, word4, word5);
    return 0;
}

