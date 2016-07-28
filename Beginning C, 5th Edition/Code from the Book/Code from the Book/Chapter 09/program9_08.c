// Program 9.8 A program to list the command line arguments
#include <stdio.h>
int main(int argc, char *argv[])
{
    printf("Program name: %s\n", argv[0]);

    for (int i = 1 ; i < argc ; ++i)
        printf("Argument %d: %s\n", i, argv[i]);

    return 0;
}
