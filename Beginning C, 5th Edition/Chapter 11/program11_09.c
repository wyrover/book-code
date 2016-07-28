// Program 11.9 Generating a Bar chart
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>                                        // For strcmp()
#include <stdlib.h>                                        // For atof()
#include <stdbool.h>

#define PAGE_HEIGHT  41
#define PAGE_WIDTH   75

typedef unsigned int uint;                                 // Type definition
typedef struct Bar Bar;                                    // Struct type definition

typedef struct Bar {
    double value;                                            // Bar value
    Bar *pNext;                                              // Pointer to next Bar
};

// Function prototypes
Bar *create_bar_list(void);
Bar *new_bar(void);
void bar_chart(Bar *pFirst, uint page_width, uint page_height, char *title);
void free_barlist_memory(Bar *pFirst);
char *chart_string(uint space, uint bar_width, char ch);
void draw_x_axis(uint length);

int main(void)
{
    Bar *pFirst = NULL;                                      // First Bar structure
    char title[80];                                          // Chart title
    printf_s("Enter the chart title: ");
    gets_s(title, sizeof(title));                            // Read chart title
    pFirst = create_bar_list();                              // Create Bar list
    bar_chart(pFirst, PAGE_WIDTH, PAGE_HEIGHT, title);       // Create Bar-chart
    free_barlist_memory(pFirst);                             // Free the memory
    return 0;
}

// Create and output the bar chart from the list
void bar_chart(Bar *pFirst, uint page_width, uint page_height, char *title)
{
    Bar *pLast = pFirst;                                     // Pointer to previous Bar
    double max = pFirst->value;                              // Maximum Bar value - 1st to start
    double min = pFirst->value;                              // Minimum Bar value - 1st to start
    double vert_step = 0.0;                                  // Unit step in vertical direction
    uint bar_count = 1;                                      // Number of bars - at least 1
    uint bar_width = 0;                                      // Width of a Bar
    uint space = 2;                                          // Spaces between bars
    char *column = NULL;                                     // Pointer to Bar column section
    char *blank = NULL;                                      // Blank string for Bar+space
    double position = 0.0;                                   // Current vertical position on chart
    bool axis = false;                                       // Indicates axis drawn

    // Find maximum and minimum of all Bar values
    while (pLast = pLast->pNext) {
        ++bar_count;              // Increment Bar count
        max = (max < pLast->value) ? pLast->value : max;
        min = (min > pLast->value) ? pLast->value : min;
    }

    // Always draw chart to horizontal axis
    if (max < 0.0) max = 0.0;

    if (min > 0.0) min = 0.0;

    vert_step = (max - min) / page_height;                   // Calculate step length

    // Calculate and check Bar width
    if ((bar_width = page_width / bar_count - space) < 1) {
        printf_s("\nPage width too narrow.\n");
        exit(1);
    }

    // Set up a string that will be used to build the columns
    if (!(column = chart_string(space, bar_width, '#'))) {
        printf_s("\nFailed to allocate memory in barchart()"
                 " - terminating program.\n");
        exit(1);
    }

    // Set up a string that will be a blank column
    if (!(blank = chart_string(space, bar_width, ' '))) {
        printf_s("\nFailed to allocate memory in barchart()"
                 " - terminating program.\n");
        exit(1);
    }

    // Draw the Bar chart. It is drawn line by line starting at the top
    printf_s("\n^ %s\n", title);                            // Output the chart title
    position = max;                                         // Start at the top

    for (uint i = 0 ; i < page_height ; ++i) {              // page_height lines for chart
        // Check if we need to output the horizontal axis
        if (position <= 0.0 && !axis) {
            draw_x_axis(bar_count * (bar_width + space));
            axis = true;
        }

        printf_s("|");                                         // Output vertical axis
        pLast = pFirst;                                        // start with the first Bar

        // For each Bar...
        for (uint bars = 1 ; bars <= bar_count ; ++bars) {
            // If position is between axis and value, output column. otherwise blank
            printf_s("%s", position <= pLast->value && position > 0.0 ||
                     position >= pLast->value && position <= 0.0 ? column : blank);
            pLast = pLast->pNext;
        }

        printf_s("\n");                                        // End the line of output
        position -= vert_step;                                 // Decrement position
    }

    if (!axis)                                               // Horizontal axis?
        draw_x_axis(bar_count * (bar_width + space));          // No, so draw it
    else
        printf_s("v\n");                                       // -y axis arrow head

    free(blank);                                             // Free blank string memory
    free(column);                                            // Free column string memory
}

// Draw horizontal axis
void draw_x_axis(uint length)
{
    printf_s("+");                                           // Start of x-axis

    for (uint x = 0 ; x < length ; ++x)
        printf_s("-");

    printf_s(">\n");                                         // End of x-axis
}

// Create a bar string of ch characters
char *chart_string(uint space, uint bar_width, char ch)
{
    char *str = malloc(bar_width + space + 1);

    if (str) {                                              // Get memory for the string
        uint i = 0;

        for (; i < space ; ++i)
            str[i] = ' ';                                       // Blank the space between bars

        for (; i < space + bar_width ; ++i)
            str[i] = ch;                                        // Enter the Bar characters

        str[i] = '\0';                                        // Add string terminator
    }

    return str;
}

// Create list of Bar objects
Bar* create_bar_list(void)
{
    Bar *pFirst = NULL;                                    // Address of the first object
    Bar *pBar = NULL;                                      // Stores address of new object
    Bar *pCurrent = NULL;                                  // Address of current object

    while (pBar = new_bar()) {
        if (pCurrent) {
            // There is a current object, so this is not the first
            pCurrent->pNext = pBar;                              // New address in pNext for current
            pCurrent = pBar;                                     // Make new one current
        } else                                                 // This is the first...
            pFirst = pCurrent = pBar;                            // ...so just save it.
    }

    return pFirst;
}

// Create a new Bar object from input
Bar *new_bar(void)
{
    static char value[80];                                   // Input buffer
    printf_s("Enter the value of the Bar, or Enter quit to end: ");
    gets_s(value, sizeof(value));                            // Read a value as a string

    if (strcmp(value, "quit") == 0)                          // quit entered?
        return NULL;                                           // then input finished

    Bar *pBar = malloc(sizeof(Bar));

    if (!pBar) {
        printf_s("Oops! Couldn't allocate memory for a bar.\n");
        exit(2);
    }

    pBar->value = atof(value);
    pBar->pNext = NULL;
    return pBar;
}

// Free memory for all Bar objects in the list
void free_barlist_memory(Bar *pBar)
{
    Bar* pTemp = NULL;

    while (pBar) {
        pTemp = pBar->pNext;                                   // Save pointer to next
        free(pBar);                                            // Free memory for current
        pBar = pTemp;                                          // Make next current
    }
}
