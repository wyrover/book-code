#include <stdio.h>
#include <Windows.h>

DWORD CTL_CODE_GEN(DWORD lngFunction)
{
    //const DWORD FILE_DEVICE_UNKNOWN = 0x22;
    //const DWORD METHOD_BUFFERED = 0;
    //const DWORD FILE_ANY_ACCESS = 0;
    return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
}

int main()
{
_s:
    DWORD dw;
    printf("Input HEX CTL_CODE: ");
    scanf("%x", &dw);
    printf("Output: %ld(0x%X)\n\n", CTL_CODE_GEN(dw), CTL_CODE_GEN(dw));
    getchar();
    goto _s;
}