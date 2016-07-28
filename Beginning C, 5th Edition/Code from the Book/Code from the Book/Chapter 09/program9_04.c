// Program 9.4 Static versus automatic variables
#include <stdio.h>

// Function prototypes
void test1(void);
void test2(void);

int main(void)
{
    for (int i = 0 ; i < 5 ; ++i) {
        test1();
        test2();
    }

    return 0;
}

// Function test1 with an automatic variable
void test1(void)
{
    int count = 0;
    printf("test1   count = %d\n", ++count);
}

// Function test2 with a static variable
void test2(void)
{
    static int count = 0;
    printf("test2   count = %d\n", ++count);
}
