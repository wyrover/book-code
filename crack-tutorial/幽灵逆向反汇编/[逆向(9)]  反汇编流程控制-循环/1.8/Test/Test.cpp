// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
int main(int argc, char* argv[])
{
    int nSum = 1;
    for (int nIndex = 0; nIndex < 10; nIndex++)
    {
        nSum += nIndex;
    }
    printf("%d\n", nSum);
	return 0;
}

