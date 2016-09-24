#include <windows.h>
#include <iostream>

using namespace std;

NTSTATUS NTAPI Proxy_LPNtOpenSection(
        OUT PHANDLE phSection,
        IN ACCESS_MASK DesiredAccess,
        IN POBJECT_ATTRIBUTES ObjectAttributes
        )
{

    if (ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer)
    {
        if (sys_wcsicmp((const short*)ObjectAttributes->ObjectName->Buffer,(const short*)L"shell32.dll") == 0
                )
        {
            static BOOL once = FALSE;
            if (!once)
            {
                once = TRUE;
                return 0xC0000034;
            }

        }
    }
    //gConsole->PrintfA("OpenSection:%ws\n", ObjectAttributes->ObjectName->Buffer);
    return gNtOpenSection(phSection,DesiredAccess,ObjectAttributes);
}


NTSYSAPI
NTSTATUS
NTAPI
Proxy_NtMapViewOfSection(
        IN HANDLE hSection,
        IN HANDLE hProcess,
        IN OUT PVOID *BaseAddress,
        IN ULONG ZeroBits,
        IN ULONG CommitSize,
        IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
        IN OUT PULONG ViewSize,
        IN SECTION_INHERIT InheritDisposition,
        IN ULONG AllocationType,
        IN ULONG Protect
        )
{
    if (hSection == /*shell32的Section*/)
        *BaseAddress = (PVOID)0x30000000;
    return gNtMapViewOfSection(hSection,hProcess,ZeroBits,CommitSize,SectionOffset,
                               ViewSize,InheritDisposition,AllocationType,Protect);
}


int main()
{
    cout << "Hello World!" << endl;
    return 0;
}

