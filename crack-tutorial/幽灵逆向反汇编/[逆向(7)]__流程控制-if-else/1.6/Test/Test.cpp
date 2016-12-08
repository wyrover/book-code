// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
int main(int argc, char* argv[])
{
    if (argc < 0)
    {
        printf("argc > 0");
    }
    else if (argc == 0)
    {
        printf("argc == 0");
    }
    else
    {
        printf("argc < 0");
    }
	return 0;
}

