
#include "stdio.h"

long add(long a, long b)
{
    long x = a, y = b;
    return (x + y);
}

int main(int argc, char* argv[])
{
    long a = 1, b = 2;
    
    printf("%d\n", add(a, b));

    return 0;
}