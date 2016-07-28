// Program 8.4 The functional approach to string sorting
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define BUF_LEN  256                         // Input buffer length
#define INIT_NSTR  2                         // Initial number of strings
#define NSTR_INCR  2                         // Increment to number of strings

char* str_in();                              // Reads a string
void str_sort(const char**, size_t);         // Sorts an array of strings
void swap(const char**, const char**);       // Swaps two pointers
void str_out(const char* const*, size_t);    // Outputs the strings
void free_memory(char**, size_t);            // Free all heap memory

// Function main - execution starts here
int main(void)
{
    size_t pS_size = INIT_NSTR;                 // count of pS elements
    char **pS = calloc(pS_size, sizeof(char*)); // Array of string pointers
    char **pTemp = NULL;                        // Temporary pointer
    size_t str_count = 0;                       // Number of strings read
    char *pStr = NULL;                          // String pointer
    printf("Enter one string per line. Press Enter to end:\n");

    while ((pStr = str_in()) != NULL) {
        if (str_count == pS_size) {
            pS_size += NSTR_INCR;

            if (!(pTemp = realloc(pS, pS_size * sizeof(char*)))) {
                printf("Memory allocation for array of strings failed.\n");
                return 2;
            }

            pS = pTemp;
        }

        pS[str_count++] = pStr;
    }

    str_sort(pS, str_count);                    // Sort strings
    str_out(pS, str_count);                     // Output strings
    free_memory(pS, str_count);                 // Free all heap memory
    return 0;
}

/******************************************************************
 *      String input routine                                      *
 *  Returns the address of the string or NULL for an empty string.*
 *  address of the string otherwise.                              *
 *  Also returns NULL if no memory is obtained or if there is an  *
 *  error reading from the keyboard.                              *
 ******************************************************************/
char* str_in(void)
{
    char buf[BUF_LEN];                          // Space to store input string

    if (!gets_s(buf, BUF_LEN)) {                // If NULL returned...
        // ...end the operation
        printf("\nError reading string.\n");
        return NULL;
    }

    if (buf[0] == '\0')                         // If empty string read...
        return NULL;                              // ...end the operation

    size_t str_len = strnlen_s(buf, BUF_LEN) + 1;
    char *pString = malloc(str_len);

    if (!pString) {                             // If no memory allocated...
        printf("Memory allocation failure.\n");
        return NULL;                              // ...end the operation
    }

    strcpy_s(pString, str_len, buf);          // Copy string read to new memory
    return pString;
}

/****************************************************
 *      String sort routine                         *
 * First argument is array of pointers to constant  *
 * strings which is of type const char*[].          *
 * Second argument is the number of elements in the *
 * pointer array – i.e. the number of strings       *
 ***************************************************/
void str_sort(const char **p, size_t n)
{
    bool sorted = false;                        // Strings sorted indicator

    while (!sorted) {                           // Loop until there are no swaps
        sorted = true;                            // Initialize to indicate no swaps

        for (int i = 0 ; i < n - 1 ; ++i) {
            if (strcmp(p[i], p[i + 1]) > 0) {
                sorted = false;                       // indicate we are out of order
                swap(&p[i], &p[i + 1]);               // Swap the string addresses
            }
        }
    }
}

/******************************************************
 *      Swap two pointers                             *
 * The arguments are type 'pointer to pointer to char'*
 * This is necessary to interchange the two pointers. *
 ******************************************************/

void swap(const char** p1, const char** p2)
{
    const char *pT = *p1;
    *p1 = *p2;
    *p2 = pT;
}

/******************************************************************
 *      String output routine                                     *
 * First argument is an array of const pointers to const strings. *
 * The second argument is  the number of the number of strings.   *
 ******************************************************************/
void str_out(const char* const* pStr, size_t n)
{
    printf("The sorted strings are:\n");

    for (size_t i = 0 ; i < n ; ++i)
        printf("%s\n", pStr[i]);                  // Display a string
}

/********************************************
 *      Free heap memory                    *
 * First argument is the array of pointers. *
 * Second argument is the number of strings *
 *  in the array.                           *
 *******************************************/
void free_memory(char **pS, size_t n)
{
    for (size_t i = 0 ; i < n ; ++i) {
        free(pS[i]);
        pS[i] = NULL;
    }

    free(pS);
    pS = NULL;
}
