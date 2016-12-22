#include "stdio.h"

int _stdcall add(int a, int b)
{
    return (a + b);
}

int main(int argc, char* argv[])
{
    return add(1, 2);
}