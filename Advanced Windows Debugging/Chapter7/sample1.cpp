/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "debug.h"
#include "testapp.h"

void Sample0()
{
    LPWSTR stringSD = L"O:SYG:BAD:(A;;FR;;;S-1-1-0)";
    PSECURITY_DESCRIPTOR sd = NULL;

    GENERIC_MAPPING rightsMapping={
        FILE_GENERIC_READ,
        FILE_GENERIC_WRITE,
        FILE_GENERIC_EXECUTE,
        FILE_ALL_ACCESS,
        };

    if (FALSE == ConvertStringSecurityDescriptorToSecurityDescriptor(stringSD, SDDL_REVISION_1, &sd, NULL))
    {
        TRACE(L"ConvertStringSecurityDescriptorToSecurityDescriptor failed ");
        return;
    }

    ImpersonateSelf(SecurityIdentification);
    STOP_ON_DEBUGGER;

    HANDLE hToken=NULL;
    if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken))
    {
        LocalFree(sd);
        RevertToSelf();
        return;
    }
    
    RevertToSelf();

    DWORD grantedAccess = 0;
    BOOL grantedAccessStatus = FALSE;
    PRIVILEGE_SET privileges[1];
    DWORD privilegesSize = sizeof(privileges);
    
    STOP_ON_DEBUGGER;
    if (FALSE == AccessCheck(sd, hToken, MAXIMUM_ALLOWED, &rightsMapping, privileges,&privilegesSize , &grantedAccess, &grantedAccessStatus))
    {
        TRACE(L"AccessCheck failed ");

    }
    CloseHandle(hToken);
    LocalFree(sd);

    if (grantedAccessStatus)
        wprintf(L"The current user has been granted 0x%x right to the object protected by %s security descriptor\n", grantedAccess, stringSD );
    else
        wprintf(L"The current user has been denied access to the object protected by %s security descriptor\n",  stringSD );
}

void Sample1()
{
    WCHAR computerName[_MAX_PATH];
    DWORD arrayLength = _MAX_PATH;


    BOOL retCode = TRUE;
    ImpersonateSelf(SecurityAnonymous);

    retCode = GetComputerNameEx(ComputerNameNetBIOS, computerName, &arrayLength);
    RevertToSelf();

    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityAnonymous.");
    }
    
    arrayLength = _MAX_PATH;
    ImpersonateSelf(SecurityIdentification);
       

    retCode = GetComputerNameEx(ComputerNameNetBIOS, computerName,   &arrayLength);
    RevertToSelf();
        
    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityIdentification.");
    }

    arrayLength = _MAX_PATH;
    ImpersonateSelf(SecurityImpersonation);

    retCode = GetComputerNameEx(ComputerNameNetBIOS, computerName,  &arrayLength);
    RevertToSelf();
        
    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityImpersonation.");
    }

    arrayLength = _MAX_PATH;
    ImpersonateSelf(SecurityDelegation);

    retCode = GetComputerNameEx(ComputerNameNetBIOS, computerName,   &arrayLength);
    RevertToSelf();

    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityDelegation.");
    }
}

