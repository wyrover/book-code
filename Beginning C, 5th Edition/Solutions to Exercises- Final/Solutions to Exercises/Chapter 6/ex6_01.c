// Exercise 6.1 Convert an integer to words
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *unit_words[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    char *teen_words[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    char *ten_words[] = {"error", "error", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    char hundred[] = " hundred";
    char and [] = " and ";
    char value_str[50] = "";
    int value = 0;                     // Integer to be converted
    int digits[] = {0, 0, 0};          // Stores digits of value entered
    int i = 0;
    printf("Enter a positive integer less than 1000: ");
    scanf_s("%d", &value);

    if (value >= 1000)
        value = 999;
    else if (value < 1)
        value = 1;

    while (value > 0) {
        digits[i++] = value % 10;
        value /= 10;
    }

    if (digits[2] > 0) {
        strcat_s(value_str, sizeof(value_str), unit_words[digits[2]]);
        strcat_s(value_str, sizeof(value_str), hundred);

        if (digits[1] > 0 || digits[0] > 0)
            strcat_s(value_str, sizeof(value_str), and);
    }

    if (digits[1] > 0) {
        if (digits[1] == 1)
            strcat_s(value_str, sizeof(value_str), teen_words[digits[0]]);
        else {
            strcat_s(value_str, sizeof(value_str), ten_words[digits[1]]);

            if (digits[0] > 0) {
                strcat_s(value_str, sizeof(value_str), " ");
                strcat_s(value_str, sizeof(value_str), unit_words[digits[0]]);
            }
        }
    } else if (digits[0] > 0)
        strcat_s(value_str, sizeof(value_str), unit_words[digits[0]]);

    printf("\n%s\n", value_str);
    return 0;
}

