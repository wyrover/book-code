// Exercise 7.3 Removing spaces and puctuation from a string
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUF_LEN  20                              // Initial length of input buffer
#define BUF_INCR 10                              // Buffer size increment

int main(void)
{
    size_t buf_len = BUF_LEN;
    char *buffer = (char*)malloc(buf_len);         // Input buffer
    char *temp = NULL;
    char *pbuffer1 = buffer;                       // Pointer to buffer position
    char *pbuffer2 = NULL;                         // Pointer to buffer position
    // Read a string
    printf("Enter a string of any length ending with a newline:\n");

    while ((*pbuffer1++ = getchar()) != '\n') {
        if ((pbuffer1 - buffer) == buf_len) {
            buf_len += BUF_INCR;

            if (!(temp = realloc(buffer, buf_len))) {
                printf("Error allocating buffer.\n");
                exit(1);
            }

            pbuffer1 = temp + (pbuffer1 - buffer);
            buffer = temp;
            temp = NULL;
        }
    }

    *pbuffer1 = '\0';                              // Append string terminator
    pbuffer1 = pbuffer2 = buffer;                  // Reset pointers to start of string

    while ((*pbuffer1) != '\0') {                  // Loop until the end of the string
        if (ispunct(*pbuffer1) || isspace(*pbuffer1)) {
            // If it's space or punctuation
            ++pbuffer1;                                // go to the next character
            continue;
        } else
            *pbuffer2++ = *pbuffer1++;                 // otherwise, copy the character
    }

    *pbuffer2 = '\0';                              // Append string terminator
    printf("With the spaces and punctuation removed, the string is now:\n%s\n", buffer);
    free(buffer);
    return 0;
}

