#include <windows.h>
#include <winternl.h>
#include <iostream>

typedef UINT (WINAPI* PENTRY_GetSystemFirmwareTable)(
        __in DWORD FirmwareTableProviderSignature,
        __in DWORD FirmwareTableID,
        __out_bcount_part_opt(BufferSize, return) PVOID pFirmwareTableBuffer,
        __in DWORD BufferSize
        );

using namespace std;

typedef struct _RawSMBIOSData
{
    BYTE	Used20CallingMethod;
    BYTE	SMBIOSMajorVersion;
    BYTE	SMBIOSMinorVersion;
    BYTE	DmiRevision;
    DWORD	Length;
    BYTE	SMBIOSTableData[0];
}RawSMBIOSData,*PRawSMBIOSData;

int main()
{
    BOOL pRet = FALSE;
    PENTRY_GetSystemFirmwareTable pGetSystemFirmwareTable =
            (PENTRY_GetSystemFirmwareTable)GetProcAddress(GetModuleHandleA("kernel32.dll"),
                                                          "GetSystemFirmwareTable");
    if (pGetSystemFirmwareTable)
    {
        int pRtSize = pGetSystemFirmwareTable('RSMB',0, NULL, 0);
        char* plpBuf = (char*)malloc(pRtSize);
        memset(plpBuf,0,pRtSize);
        int prtStatas = pGetSystemFirmwareTable('RSMB',0,plpBuf,pRtSize);
        if (prtStatas <= pRtSize)
        {
            RawSMBIOSData* pRawSMBInfo = (RawSMBIOSData*)plpBuf;
            for (int i=0;i<pRawSMBInfo->Length;i++)
            {
                if (pRawSMBInfo->SMBIOSTableData[i] == 'V')
                {
                    if (strnicmp((const char*)pRawSMBInfo->SMBIOSTableData[i],"vmware",6) == 0)
                    {
                        cout << "发现调试器!" << endl;
                        break;
                    }
                }
            }

        }
        free(plpBuf);
    }
    return pRet;
}

