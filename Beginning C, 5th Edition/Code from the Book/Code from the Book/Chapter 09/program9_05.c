// Program 9.5 Global variables
#include <stdio.h>

int count = 0;                         // Declare a global variable

// Function prototypes
void test1(void);
void test2(void);

int main(void)
{
    int count = 0;                      // This hides the global count

    for (; count < 5 ; ++count) {
        test1();
        test2();
    }

    return 0;
}

// Function test1 using the global variable
void test1(void)
{
    printf("test1   count = %d\n", ++count);
}

// Function test2 using a static variable
void test2(void)
{
    static int count;                   // This hides the global count
    printf("test2   count = %d\n", ++count);
}
