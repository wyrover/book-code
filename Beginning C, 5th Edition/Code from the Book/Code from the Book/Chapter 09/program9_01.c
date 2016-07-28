// Program 9.1 Pointing to functions
#include <stdio.h>

// Function prototypes
int sum(int, int);
int product(int, int);
int difference(int, int);

int main(void)
{
    int a = 10;                         // Initial value for a
    int b = 5;                          // Initial value for b
    int result = 0;                     // Storage for results
    int (*pfun)(int, int);              // Function pointer declaration
    pfun = sum;                         // Points to function sum()
    result = pfun(a, b);                // Call sum() through pointer
    printf("pfun = sum             result = %2d\n", result);
    pfun = product;                     // Points to function product()
    result = pfun(a, b);                // Call product() through pointer
    printf("pfun = product         result = %2d\n", result);
    pfun = difference;                  // Points to function difference()
    result = pfun(a, b);                // Call difference() through pointer
    printf("pfun = difference      result = %2d\n", result);
    return 0;
}

int sum(int x, int y)
{
    return x + y;
}

int product(int x, int y)
{
    return x * y;
}

int difference(int x, int y)
{
    return x - y;
}
