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
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <process.h>
#include <malloc.h>

int global = 0;

DWORD ex_filter()
{
    return EXCEPTION_EXECUTE_HANDLER;
}

void try_except()
{
    __try
    {
        *((int *) 0) = 0;
    }
    __except(ex_filter())
    {
    global = 1;
    }
}
void try_finally()
{
    __try
    {
        *((int *) 0) = 0;
    }
    __finally
    {
    global = 2;
    }

}

void try_except_finally()
{
    __try
    {
        __try
        {
            *((int *) 0) = 0;
        }
        __finally
        {
        global = 3;
        }

    }
    __except(ex_filter())
    {
    global = 4;
    }
    __try
    {
        *((int *) 0) = 0;
    }
    __except(ex_filter())
    {
    global = 5;
    }

}

/*void try_catch()
{
    try
    {
        *((int *) 0) = 0;
    }
    catch(int&)
    {
    global = 6;    
    }
}*/

class SetGlobal
{
public:
    ~SetGlobal()
    {
        global = 6;
    }
};

/*void try_destruct()
{
    SetGlobal setGlobal;
    * ((int *) 0) = 0;
}*/
