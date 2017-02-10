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

#include "testapp.h"

void Sample4()
{
        WCHAR computerName[_MAX_PATH];
        DWORD arrayLength = _MAX_PATH*sizeof(WCHAR);


    BOOL retCode = TRUE;
    ImpersonateSelf(SecurityAnonymous);

    retCode = GetComputerNameExW(ComputerNameNetBIOS, computerName, &arrayLength);
            RevertToSelf();

    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityAnonymous.");
    }
    
        arrayLength = _MAX_PATH*sizeof(WCHAR);
        ImpersonateSelf(SecurityIdentification);
       

    retCode = GetComputerNameExW(ComputerNameNetBIOS, computerName,   &arrayLength);
        RevertToSelf();
        
    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityIdentification.");
    }

        arrayLength = _MAX_PATH*sizeof(WCHAR);
        ImpersonateSelf(SecurityImpersonation);

    retCode = GetComputerNameExW(ComputerNameNetBIOS, computerName,  &arrayLength);
        RevertToSelf();
        
    if (retCode != TRUE)
    {
        TRACE(L"GetComputerName fails with token @ SecurityImpersonation.");
    }

        arrayLength = _MAX_PATH*sizeof(WCHAR);
        ImpersonateSelf(SecurityDelegation);

        retCode = GetComputerNameExW(ComputerNameNetBIOS, computerName,   &arrayLength);
        RevertToSelf();

        if (retCode != TRUE)
        {
            TRACE(L"GetComputerName fails with token @ SecurityDelegation.");
        }
}

