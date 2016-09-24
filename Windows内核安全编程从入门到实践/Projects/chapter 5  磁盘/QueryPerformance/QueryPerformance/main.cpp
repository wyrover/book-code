#include "stdio.h"
#include "windows.h"

int main()
{
    HANDLE hFile;
    DISK_PERFORMANCE OutBuffer = {0};
    ULONG OutBufferSize = sizeof(DISK_PERFORMANCE), dwRet;
    BOOL bRet;
    hFile = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ,
                        FILE_SHARE_READ, NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        printf("CreateFile Failure!err:%d\n", GetLastError());
        return 0;
    }

    bRet = DeviceIoControl(hFile, IOCTL_DISK_PERFORMANCE, NULL, 0, (LPVOID)&OutBuffer, OutBufferSize, &dwRet, NULL);

    if (FALSE == bRet) {
        printf("DeviceIoControl Failure!err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    printf("\
BytesRead: %I64u\nBytesWritten:%I64u\nReadTime:%I64u\nWriteTime:%I64u\nIdleTime:%I64u\n\
ReadCount:%d\nWriteCount:%d\nQueueDepth:%d\nSplitCount:%d\nQueryTime::%I64u\n\
StorageDeviceNumber:%x\nStorageManagerName:%ws\n",
           OutBuffer.BytesRead, OutBuffer.BytesWritten, OutBuffer.ReadTime, OutBuffer.WriteTime, OutBuffer.IdleTime,
           OutBuffer.ReadCount, OutBuffer.WriteCount, OutBuffer.QueueDepth, OutBuffer.SplitCount,
           OutBuffer.QueryTime, OutBuffer.StorageDeviceNumber, OutBuffer.StorageManagerName
          );
    CloseHandle(hFile);
    return 0;
}