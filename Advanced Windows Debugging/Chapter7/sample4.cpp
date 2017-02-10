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
#include "debug.h"
#include "testapp.h"

void Sample4Set()
{
    wprintf(L"Creating application settings ...\n");
    HKEY softwareKey = NULL;
    LONG retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software", 0, MAXIMUM_ALLOWED, &softwareKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegOpenKeyEx failed.", retCode);
        return;
    } 

    HKEY bookKey = NULL;
    retCode = RegCreateKey(softwareKey, L"Advanced Windows Debugging", &bookKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegCreateKey failed.", retCode);
        RegCloseKey(softwareKey);
        return;
    } 
    else
    {
        wprintf(L"Registry key HKEY_LOCAL_MACHINE\\Software\\Advanced Windows Debugging created\n");
    }

    RegCloseKey(bookKey);
    RegCloseKey(softwareKey);

    WCHAR fileName[MAX_PATH];
    if (!ExpandEnvironmentStrings(L"%ProgramFiles%\\sample4.test", fileName,MAX_PATH))
    {
        TRACE_ERR(L"ExpandEnvironmentStrings failed.", retCode);
        return;
    } 

    wprintf(L"Create file %s \n", fileName);
    HANDLE hFile = CreateFile(fileName,GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0);
    if (hFile == INVALID_HANDLE_VALUE)
    {
           TRACE(L"CreateFile failed. ");
    }
    else
    {
        wprintf(L"File %s created\n",fileName);
        CloseHandle(hFile);
    }

    wprintf(L"Creating application done.\n");
}

void Sample4Get()
{
    wprintf(L"Retrieving application settings ...\n");
    HKEY softwareKey = NULL;
    LONG retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software", 0, GENERIC_READ, &softwareKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegOpenKeyEx failed.", retCode);
        return;
    }

    HKEY bookKey = NULL;
    retCode = RegOpenKey(softwareKey, L"Advanced Windows Debugging", &bookKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegOpenKey failed.", retCode);
        RegCloseKey(softwareKey);
        return;
    }
    else
        wprintf(L"Registry key HKEY_LOCAL_MACHINE\\Software\\Advanced Windows Debugging found\n");

    RegCloseKey(bookKey);
    RegCloseKey(softwareKey);

    WCHAR fileName[MAX_PATH];
    if (!ExpandEnvironmentStrings(L"%ProgramFiles%\\sample4.test", fileName,MAX_PATH))
    {
        TRACE_ERR(L"ExpandEnvironmentStrings failed.", retCode);
        return;
    } 
    wprintf(L"Create file %s \n", fileName);
    HANDLE hFile = CreateFile(fileName,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE(L"CreateFile failed. ");
    }
    else
    {
        wprintf(L"File %s found\n",fileName);
        CloseHandle(hFile);
    }

    wprintf(L"Retrieving application done.\n");
}

