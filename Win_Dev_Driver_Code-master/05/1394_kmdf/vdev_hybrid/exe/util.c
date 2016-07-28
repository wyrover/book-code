/*++

Copyright (c) 1997-1998  Microsoft Corporation

Module Name: 

    util.c

Abstract

    Misc. functions that are convient to have around.
--*/

#define _UTIL_C
#include "pch.h"
#undef _UTIL_C

HANDLE
OpenDevice (
            HANDLE       hWnd,
            __in PSTR    szDeviceName)
{
    HANDLE  hDevice;
    CHAR    tmpBuff[STRING_SIZE];

    hDevice = CreateFile (
        szDeviceName,
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (INVALID_HANDLE_VALUE == hDevice) 
    {

        TRACE(TL_ERROR, (hWnd, "Failed to open device!\n"));

        if (hWnd) 
        {
            StringCbPrintf (
                tmpBuff, 
                (STRING_SIZE * sizeof(tmpBuff[0])), 
                "Error Opening Device %d\r\n\r\n", 
                GetLastError());

            WriteTextToEditControl (hWnd, tmpBuff);
        }
    }

    return (hDevice);
} // OpenDevice

DWORD
WINAPI
RemoveVirtualDriver (
                     HWND           hWnd,
                     PVIRT_DEVICE   pVirtualDevice,
                     ULONG          BusNumber)
{
    HANDLE                      hDevice;
    ULONG                       ulStrLen;
    size_t                      retVal = 0;
    UCHAR                       BusName[16] = "\\\\.\\1394BUS";
    PIEEE1394_API_REQUEST       p1394ApiReq;
    PIEEE1394_VDEV_PNP_REQUEST  pDevPnpReq;
    DWORD                       dwBytesRet;
    DWORD                       dwRet;
    ULONG                       p1394ApiReqLen = 0;

    if (!pVirtualDevice->DeviceID)
    {
        dwRet = ERROR_INVALID_PARAMETER;
        goto Exit_RemoveVirtualDriver;
    }

    _itoa (BusNumber, (CHAR *) &BusName[11], 10);

    hDevice = OpenDevice (hWnd, (PSTR) &BusName);
    if (hDevice != INVALID_HANDLE_VALUE) 
    {

        //taking care of NULL termination character
        retVal = strlen(pVirtualDevice->DeviceID) + 1;

        SizeTToULong (retVal, &ulStrLen);


        if ( pVirtualDevice->fulFlags & IEEE1394_REQUEST_FLAG_UNICODE ) 
        {
            p1394ApiReq = \
                (PIEEE1394_API_REQUEST) LocalAlloc (
                LPTR, 
                sizeof(IEEE1394_API_REQUEST)+(ulStrLen * sizeof(WCHAR)));

            p1394ApiReqLen = \
                sizeof(IEEE1394_API_REQUEST)+(ulStrLen * sizeof(WCHAR));
        } 
        else 
        {
            p1394ApiReq = (PIEEE1394_API_REQUEST) LocalAlloc (
                LPTR, 
                sizeof(IEEE1394_API_REQUEST)+ulStrLen);

            p1394ApiReqLen = sizeof(IEEE1394_API_REQUEST)+ ulStrLen;
        }

        p1394ApiReq->RequestNumber = IEEE1394_API_REMOVE_VIRTUAL_DEVICE;
        p1394ApiReq->Flags = 0;

        pDevPnpReq = &p1394ApiReq->u.RemoveVirtualDevice;

        pDevPnpReq->fulFlags = pVirtualDevice->fulFlags;
        pDevPnpReq->Reserved = 0;
        pDevPnpReq->InstanceId = pVirtualDevice->InstanceID;
        
        if (pVirtualDevice->fulFlags & IEEE1394_REQUEST_FLAG_UNICODE) 
        {
            StringCchCopyNW (
                (LPWSTR)&pDevPnpReq->DeviceId,
                ulStrLen,
                (LPWSTR)pVirtualDevice->DeviceID, 
                ulStrLen);

        
            dwRet = DeviceIoControl (
                hDevice,
                IOCTL_IEEE1394_API_REQUEST,
                p1394ApiReq,
                p1394ApiReqLen,
                NULL,
                0,
                &dwBytesRet,
                NULL);
        } 
        else 
        {
            StringCchCopyNA (
                (STRSAFE_LPSTR) &pDevPnpReq->DeviceId,
                ulStrLen,
                (STRSAFE_LPSTR) pVirtualDevice->DeviceID, 
                ulStrLen);

            dwRet = DeviceIoControl (
                hDevice,
                IOCTL_IEEE1394_API_REQUEST,
                p1394ApiReq,
                p1394ApiReqLen,
                NULL,
                0,
                &dwBytesRet,
                NULL);
        }

        if (!dwRet) 
        {

            dwRet = GetLastError();
        }
        else 
        {

            dwRet = ERROR_SUCCESS;
        }

        // free resources

        CloseHandle (hDevice);
        LocalFree(p1394ApiReq);
    }
    else 
    {
        dwRet = ERROR_INVALID_HANDLE;
    }

Exit_RemoveVirtualDriver:

    return dwRet;
}

DWORD
WINAPI
AddVirtualDriver (
                  HWND                hWnd,
                  PVIRT_DEVICE    pVirtualDevice,
                  ULONG               BusNumber)
{
    HANDLE                      hDevice;
    ULONG                       ulStrLen;
    size_t                      retVal = 0;
    UCHAR                       BusName[16] = "\\\\.\\1394BUS";
    PIEEE1394_API_REQUEST       p1394ApiReq;
    PIEEE1394_VDEV_PNP_REQUEST  pDevPnpReq;
    DWORD                       dwBytesRet;
    DWORD                       dwRet;
    ULONG                       p1394ApiReqLen = 0;

    TRACE (TL_TRACE, (hWnd, "AddVirtual Driver\n"));

    _itoa (BusNumber, (CHAR *) &BusName[11], 10);
    TRACE (TL_TRACE, (hWnd, "Bus: %s\n", BusName));

    hDevice = OpenDevice (hWnd, (PSTR) &BusName);
    if (hDevice != INVALID_HANDLE_VALUE)
    {

        //taking care of NULL terminating character
        retVal = strlen(pVirtualDevice->DeviceID) + 1;

        SizeTToULong (retVal, &ulStrLen);

        if ( pVirtualDevice->fulFlags & IEEE1394_REQUEST_FLAG_UNICODE ) 
        {
            p1394ApiReq = (PIEEE1394_API_REQUEST) LocalAlloc (
                LPTR, 
                sizeof(IEEE1394_API_REQUEST)+(ulStrLen * sizeof(WCHAR)));

            p1394ApiReqLen = \
                sizeof(IEEE1394_API_REQUEST)+(ulStrLen * sizeof(WCHAR));
        } 
        else 
        {
            p1394ApiReq = (PIEEE1394_API_REQUEST) LocalAlloc(LPTR, 
                sizeof(IEEE1394_API_REQUEST)+ulStrLen);

            p1394ApiReqLen = sizeof(IEEE1394_API_REQUEST)+ ulStrLen;
        }

        p1394ApiReq->RequestNumber = IEEE1394_API_ADD_VIRTUAL_DEVICE;
        p1394ApiReq->Flags = pVirtualDevice->fulFlags;

        pDevPnpReq = &p1394ApiReq->u.AddVirtualDevice;

        pDevPnpReq->fulFlags = pVirtualDevice->fulFlags;
        pDevPnpReq->Reserved = 0;
        pDevPnpReq->InstanceId = pVirtualDevice->InstanceID;

        if ( pVirtualDevice->fulFlags & IEEE1394_REQUEST_FLAG_UNICODE ) 
        {
            StringCchCopyNW (
                (LPWSTR)&pDevPnpReq->DeviceId,
                ulStrLen,
                (LPWSTR)pVirtualDevice->DeviceID, 
                ulStrLen);


            dwRet = DeviceIoControl (
                hDevice,
                IOCTL_IEEE1394_API_REQUEST,
                p1394ApiReq,
                p1394ApiReqLen,
                NULL,
                0,
                &dwBytesRet,
                NULL);
        } 
        else 
        {
            StringCchCopyNA (
                (STRSAFE_LPSTR) &pDevPnpReq->DeviceId,
                ulStrLen,
                (STRSAFE_LPSTR) pVirtualDevice->DeviceID, 
                ulStrLen);

            dwRet = DeviceIoControl (
                hDevice,
                IOCTL_IEEE1394_API_REQUEST,
                p1394ApiReq,
                p1394ApiReqLen,
                NULL,
                0,
                &dwBytesRet,
                NULL);
        }

        if (!dwRet)
        {
            
            dwRet = GetLastError();
            TRACE (
                TL_WARNING, 
                (hWnd, 
                "Failed IOCTL_IEEE1394_API_REQEST %d\n", 
                dwRet));
        }
        else 
        {
            TRACE (TL_WARNING, (hWnd, "Succeeded IOCTL_IEEE1394_API_REQEST\n"));
            dwRet = ERROR_SUCCESS;
        }

        // free resources

        CloseHandle (hDevice);
        LocalFree(p1394ApiReq);
    }
    else 
    {
        dwRet = ERROR_INVALID_HANDLE;
    }

    return dwRet;
}

DWORD
FillDeviceList (
                HWND            hWnd,
                LPGUID          InterfaceGuid,
                PDEVICE_DATA    pDeviceData)
{
    ULONG dwError = ERROR_SUCCESS;
    HDEVINFO HardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
    ULONG Length, RequiredLength = 0;
    BOOL bResult;

    INT i = 0;

    // 
    // We'll only be looking for one device right now.
    //

    TRACE (TL_TRACE, (hWnd, "Entering FillDeviceList\n"));


    HardwareDeviceInfo = SetupDiGetClassDevs (
        InterfaceGuid, 
        NULL, 
        NULL,
        (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

    if (INVALID_HANDLE_VALUE == HardwareDeviceInfo) 
    {
        dwError = GetLastError();
        TRACE (TL_WARNING, (hWnd, "Failed SetupDiGetClassDevs %d\n", dwError));
        return dwError;
    }

    DeviceInterfaceData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

    bResult = SetupDiEnumDeviceInterfaces (
        HardwareDeviceInfo, 
        0, 
        InterfaceGuid, 
        0,
        &DeviceInterfaceData);

    if (!bResult) 
    {
        dwError = GetLastError();
        TRACE (
            TL_WARNING, 
            (hWnd, 
            "SetupDiEnumDeviceInterfaces failed: %d.\n", 
            GetLastError()));

        SetupDiDestroyDeviceInfoList (HardwareDeviceInfo);

        return dwError;
    }

    SetupDiGetDeviceInterfaceDetail (
        HardwareDeviceInfo, 
        &DeviceInterfaceData, 
        NULL, 
        0,
        &RequiredLength, 
        NULL);

    DeviceInterfaceDetailData = \
        (PSP_DEVICE_INTERFACE_DETAIL_DATA) LocalAlloc (
        LMEM_FIXED, 
        RequiredLength);

    if (NULL == DeviceInterfaceDetailData) 
    {
        dwError = ERROR_NOT_ENOUGH_MEMORY;
        TRACE (TL_WARNING, (hWnd, "Failed Allocate DeviceInterfaceDetail\n"));

        SetupDiDestroyDeviceInfoList (HardwareDeviceInfo);       

        return dwError;
    }

    DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    Length = RequiredLength;

    bResult = SetupDiGetDeviceInterfaceDetail (
        HardwareDeviceInfo, 
        &DeviceInterfaceData,
        DeviceInterfaceDetailData, 
        Length, 
        &RequiredLength,
        NULL);

    if (!bResult) 
    {
        dwError = GetLastError();
        TRACE (
            TL_WARNING, 
            (hWnd, 
            "Failed SetupDiGetDeviceInterfaceDetail %d\n", 
            dwError));
       
        LocalFree (DeviceInterfaceDetailData);
        SetupDiDestroyDeviceInfoList (HardwareDeviceInfo);  

        return dwError;
    }

    pDeviceData->numDevices++;

    StringCbCopy(
        pDeviceData->deviceList[i].DeviceName, 
        sizeof(pDeviceData->deviceList[i].DeviceName),
        DeviceInterfaceDetailData->DevicePath);
    
    LocalFree(DeviceInterfaceDetailData);
    SetupDiDestroyDeviceInfoList (HardwareDeviceInfo);  
   

    TRACE (TL_TRACE, (hWnd, "Exiting FillDeviceList %d\n", dwError));
    return dwError;
}

DWORD
SendRequest (
             IN DWORD Ioctl,
             IN PVOID InputBuffer,
             IN ULONG InputBufferSize,
             IN OUT PVOID OutputBuffer,
             IN OUT ULONG OutputBufferSize,
             IN OUT LPDWORD bytesReturned)
{
    // 
    // Everything out of this test application will be going synchronously.
    // 

    if  (!DeviceIoControl (
        g_hTestDevice, 
        Ioctl,
        InputBuffer,
        InputBufferSize, 
        OutputBuffer, 
        OutputBufferSize, 
        bytesReturned, 
        NULL))
    {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}

VOID
WriteTextToEditControl (
                       HWND hWndEdit, 
                       __in PCHAR str)
{
    INT     iLength;

    // get the end of buffer for edit control
    iLength = GetWindowTextLength (hWndEdit);

    // set current selection to that offset
    SendMessage (hWndEdit, EM_SETSEL, iLength, iLength);

    // add text
    SendMessage (hWndEdit, EM_REPLACESEL, 0, (LPARAM) str);

} // WriteTextToEditControl

//
// Generic singly linked list routines.
//

//***********************************************************************
//
void 
InsertTailList (
               PLIST_NODE head, 
               PLIST_NODE entry)
{
PLIST_NODE pCurrent = head;

    entry->pNext = 0;
    while(pCurrent->pNext)
        pCurrent = pCurrent->pNext;
    pCurrent->pNext = entry;

}

//***********************************************************************
//
BOOL 
RemoveEntryList (
                 PLIST_NODE head, 
                 PLIST_NODE entry)
{
PLIST_NODE pCurrent = head;

    while(pCurrent->pNext != entry){
        pCurrent = pCurrent->pNext;
        if(pCurrent == 0) return FALSE;
    }
    pCurrent->pNext = entry->pNext;
    return TRUE;
}
    


//***********************************************************************
//
void 
InsertHeadList (
                PLIST_NODE head, 
                PLIST_NODE entry)
{
    entry->pNext = head->pNext;
    head->pNext = entry;
}

//****************************************************************************
//
BOOL 
IsNodeOnList (
              PLIST_NODE head, 
              PLIST_NODE entry)
{
PLIST_NODE pCurrent = head;

    while(pCurrent->pNext != entry){
        pCurrent = pCurrent->pNext;
        if(pCurrent == 0) return FALSE;
    }
    return TRUE;
}



