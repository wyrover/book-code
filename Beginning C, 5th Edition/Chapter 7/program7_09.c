// Program 7.9  Multidimensional arrays and pointers
#include <stdio.h>

int main(void)
{
    char board[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };
    char *pboard = *board;             // A pointer to char

    for (int i = 0 ; i < 9 ; ++i)
        printf(" board: %c\n", *(pboard + i));

    return 0;
}
