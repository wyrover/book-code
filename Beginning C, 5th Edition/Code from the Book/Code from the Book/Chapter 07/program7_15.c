// Program 7.15 An improved calculator
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>                                             // Standard input/output
#include <string.h>                                            // For string functions
#include <ctype.h>                                             // For classifying characters
#include <stdlib.h>                                            // For converting strings to numeric values
#include <stdbool.h>                                           // For bool values
#include <math.h>                                              // For power() function
#define BUF_LEN 256                                            // Length of input buffer

int main(void)
{
    char buf[BUF_LEN];                                           // Input expression
    char op = 0;                                                 // Stores an operator
    size_t index = 0;                                            // Index of the current character in buf
    size_t to = 0;                                               // To index for copying buf to itself
    size_t buf_length = 0;                                       // Length of the string in buf
    double result = 0.0;                                         // The result of an operation
    double number = 0.0;                                         // Stores the value of right operand
    char* endptr = NULL;                                         // Stores address of character following a number
    printf("To use this calculator, enter any expression with"
           " or without spaces.");
    printf("\nAn expression may include the operators");
    printf(" +, -, *, /, %%, or ^(raise to a power).");
    printf("\nUse = at the beginning of a line to operate on ");
    printf("\nthe result of the previous calculation.");
    printf("\nEnter quit to stop the calculator.\n\n");
    // The main calculator loop
    char *ptr = NULL;

    while (true) {
        ptr = fgets(buf, BUF_LEN, stdin);

        if (!ptr) {                                                // Check for read error
            printf("Error reading input.\n");
            return 1;
        }

        if (strcmp(buf, "quit\n") == 0) break;                     // Quit check

        buf_length = strnlen_s(buf, BUF_LEN);                      // Get the input string length
        buf[--buf_length] = '\0';                                  // Remove newline at the end

        // Remove spaces from the input by copying the string to itself
        for (to = 0, index = 0 ; index <= buf_length ; ++index) {
            if (*(buf + index) != ' ')                               // If it's not a space...
                *(buf + to++) = *(buf + index);                        // ...copy the character
        }

        buf_length = strnlen_s(buf, BUF_LEN);                      // Get the new string length
        index = 0;                                                 // Start at the first character

        if (buf[index] == '=')                                      // If there's = ...
            index++;                                                  // ...skip over it
        else {
            // No =, so look for left operand
            result = strtod(buf + index, &endptr);                   // Convert & store the number
            index = endptr - buf;                                    // Get index for next character
        }

        // Now look for 'op number' combinations
        while (index < buf_length) {
            op = *(buf + index++);                                   // Get the operator
            number = strtod(buf + index, &endptr);                   // Convert & store the number
            index = endptr - buf;                                    // Get index for next character

            // Execute operation, as 'result op= number'
            switch (op) {
            case '+':                                             // Addition
                result += number;
                break;

            case '-':                                             // Subtraction
                result -= number;
                break;

            case '*':                                             // Multiplication
                result *= number;
                break;

            case '/':                                             // Division

                // Check second operand for zero
                if (number == 0) printf("\n\n\aDivision by zero error!\n");
                else
                    result /= number;

                break;

            case '%':                                             // Modulus operator - remainder

                // Check second operand for zero
                if ((long long)number == 0LL) printf("\n\n\aDivision by zero error!\n");
                else
                    result = (double)((long long)result % (long long)number);

                break;

            case '^':                                             // Raise to a power
                result = pow(result, number);
                break;

            default:                                              // Invalid operation or bad input
                printf("\n\n\aIllegal operation!\n");
                break;
            }
        }

        printf("= %f\n", result);                                  // Output the result
    }

    return 0;
}

