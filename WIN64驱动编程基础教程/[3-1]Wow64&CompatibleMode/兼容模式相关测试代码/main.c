#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef long (__stdcall *RTLGETVERSION)(POSVERSIONINFO);

int main()
{
    RTLGETVERSION RtlGetVersion = (RTLGETVERSION)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    OSVERSIONINFO osv1 = {0}, osv2 = {0};
    //way 1
    osv1.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osv1);
    printf("Get Build Number by GetVersionEx: %ld\n", osv1.dwBuildNumber);
    //way 2
    osv2.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    RtlGetVersion(&osv2);
    printf("Get Build Number by RtlGetVersion: %ld\n", osv2.dwBuildNumber);
    //show info
    getchar();
    return 0;
}
