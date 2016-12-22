// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

typedef struct _tagTEST
{
    int i;
    int j;
    int k;
} TEST, *PTEST;
int Test(int* nA, int *nB)
{
    int i = 0;
    int j = 0;
    char cChar;
    scanf("%d", &i);
    printf("%d\n", i);
    scanf("%d", &j);
    printf("%d\n", j);
    scanf("%c", &cChar);
    printf("%c", cChar);
    (*nA)++;
    (*nB)++;
    
    printf("%d %d", *nA, *nB);
    return *nA;
}

TEST TestStruct()
{
    TEST Test1;
    Test1.i = 3;
    Test1.j = 2;
    Test1.k = 1;
    return Test1;
}
int main(int argc, char* argv[])
{
    TEST Test1 = {0};
    Test1 = TestStruct();
    int i = 3;
    int j = 2;
    Test(&i, &j);
    return 0;
}

