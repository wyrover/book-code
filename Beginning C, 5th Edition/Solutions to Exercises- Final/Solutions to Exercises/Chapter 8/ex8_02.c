// Exercise 8.2 A function to return a string representation of an integer
#include <stdio.h>
#include <stdbool.h>

#define STR_LEN 6                                // Length of string to store value string

// Converts an integer to a string. Caller must allocate string array.
// Function returns the string to allow use of the function in an expression.
char* itoa(int n, char str[], size_t size)
{
    size_t i = 0;                                   // character count
    bool negative = n < 0;                          // Indicates negative integer
    int length = 0;                                 // Length of string
    char temp = '0';                                // Temporary storage

    if (negative)                                   // If it's negative...
        n = -n;                                       // ...make it positive

    // Generate digit characters in reverse order
    do {
        if (size == i + 1 + (negative ? 1 : 0)) {
            printf("String not long enough.\n");
            return NULL;
        }

        str[i++] = '0' + n % 10;                     // Create a rightmost digit
        n /= 10;                                     // Remove the digit
    } while (n > 0);                                // Go again if there's more digits

    if (negative)                                  // If it was negative...
        str[i++] = '-';                              // ...append minus

    str[i] = '\0';                                 // Append terminator
    length = i;                                    // Save the length including null character

    // Now reverse the string in place by switching first and last,
    // second and last but one, third and last but two, etc.
    for (i = 0 ; i < length / 2 ; ++i) {
        temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }

    return str;                                    // Return the string
}


int main(void)
{
    char str[STR_LEN];                            // Stores string representation of an integer
    long testdata[] = { 30L, -98L, 0L, -1L, 999L, -12345L, 12345L};

    for (int i = 0 ; i < sizeof testdata / sizeof(long) ; ++i) {
        if (itoa(testdata[i], str, sizeof(str)))
            printf("Integer value is %d, string is %s\n", testdata[i], str);
    }

    return 0;
}

