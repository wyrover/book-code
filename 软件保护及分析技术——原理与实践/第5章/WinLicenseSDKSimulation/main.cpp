#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "Q:/ToolKit/WL/WinlicenseSDK/Include/C/winlicensesdk.h"
using namespace std;

int main()
{
    int Status = WLRegGetStatus(NULL);
    if (Status == wlIsRegistered)
        printf("ÒÑ×¢²á!\n");
    else
        printf("Î´×¢²á!\n");
    getchar();
    return 0;
}


int __stdcall myWLRegGetStatus()
{
    return wlIsRegistered;
}
