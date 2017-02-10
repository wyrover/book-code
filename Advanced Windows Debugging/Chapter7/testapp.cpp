/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "menu.h"
#include "testapp.h"

void __cdecl wmain (int argc, WCHAR* args[])
{
    OPTIONS options[]={
        {L'0',L"To use Security Descriptor APIs",Sample0},
        {L'1',L"To run Impersonation test",Sample1},
        {L'2',L"To run Default SD test",Sample2},
        {L'3',L"To run MAXIMUM_ALLOWED test",Sample3},
        {L'4',L"To set application configuration information",Sample4Set},
        {L'5',L"To read application configuration information",Sample4Get},
        {L'x',L"To exit",NULL},
    }; 

    AppInfo appInfo=AppInfo(options);
    appInfo.Loop();
}
