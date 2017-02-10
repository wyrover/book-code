#include "stdafx.h"
#include "ioctrl.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        CHandle shLeakDrv;
        DWORD dwReturned, n;

        ChkProlog();

        shLeakDrv.Attach(CreateFile(
            L"\\\\.\\" DRV_CONTROL_DEVICE_NAME,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            ));
        if (shLeakDrv == INVALID_HANDLE_VALUE)
        {
            shLeakDrv.Detach();
            ChkWin32Error(::GetLastError());
        }

        for (n = 0; n < NUM_CALLS; n++)
        {
            ChkWin32(DeviceIoControl(
                shLeakDrv,
                IOCTL_DRV_TEST_API,
                NULL,
                0,
                NULL,
                0,
                &dwReturned,
                NULL
                ));
        }

        ChkNoCleanup();
    }

private:
    static const DWORD NUM_CALLS = 100;
};

int
__cdecl
wmain(
    )
{
    HRESULT hr;

    hr = CMainApp::MainHR();
    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return 0;
}
