/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#pragma auto_inline(off)
extern int CalledFunction1(int a);
extern int CalledFunction2(int a,int b);
extern int CalledFunction3(int a,int b,int c);
extern int CalledFunction4(int a,int b,int c,int d);
extern int CalledFunction5(int a,int b,int c,int d,int e);

int Function1(int a)
{
    CalledFunction1(a);
    return 1;
}

int Function2(int a, int b)
{
    CalledFunction2(a,b);
    return 2;
}

int Function3(int a, int b, int c)
{
    CalledFunction3(a,b,c);
    return 3;
}

int Function4(int a, int b, int c, int d)
{
    CalledFunction4(a,b,c,d);
    return 4;
}

int Function5(int a, int b, int c, int d, int e)
{
    CalledFunction5(a,b,c,d,e);
    return 5;
}
