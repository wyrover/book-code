#include "windows.h"

BYTE    b       =   0x12;
WORD    w       =   0x1234;
DWORD   dw      =   0x12345678;
char    str[]   =   "abcde";

int main(int argc, char *argv[])
{
    BYTE    lb  =   b;
    WORD    lw  =   w;
    DWORD   ldw =   dw;
    char    *lstr  =   str;

    return 0;
}