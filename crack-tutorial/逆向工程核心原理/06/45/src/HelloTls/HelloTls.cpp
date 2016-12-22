#include <windows.h>

#pragma comment(linker, "/INCLUDE:__tls_used")

void NTAPI TLS_CALLBACK(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    if( IsDebuggerPresent() )
    {
        MessageBoxA(NULL, "Debugger Detected!", "TLS Callback", MB_OK);
        ExitProcess(1);
    }
}

#pragma data_seg(".CRT$XLX")
    PIMAGE_TLS_CALLBACK pTLS_CALLBACKs[] = { TLS_CALLBACK, 0 };
#pragma data_seg()

int main(void)
{
    MessageBoxA(NULL, "Hello :)", "main()", MB_OK);
}
