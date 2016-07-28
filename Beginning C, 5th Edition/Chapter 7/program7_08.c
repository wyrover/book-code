// Program 7.8  Getting values in a two-dimensional array
#include <stdio.h>

int main(void)
{
    char board[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };

    // List all elements of the array
    for (int i = 0 ; i < 9 ; ++i)
        printf(" board: %c\n", *(*board + i));

    return 0;
}
