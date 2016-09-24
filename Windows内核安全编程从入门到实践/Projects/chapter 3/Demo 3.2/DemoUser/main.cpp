#include "windows.h"
#include "stdio.h"

#define IOCTL_BUFFERED_IO CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_INDIRECT_IO CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_NEITHER_IO CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_NEITHER ,FILE_ANY_ACCESS)
#define IOCTL_SHARE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN,0x803,METHOD_BUFFERED,FILE_ANY_ACCESS)

int main()
{
    HANDLE hFile;
    char Buffer[20] = {0};
    BOOL bRet;
    DWORD dwRet;
    PVOID pShareAddr = NULL;
    hFile = CreateFileA(
                "\\\\.\\Demo0",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

    if (INVALID_HANDLE_VALUE == hFile) {
        printf("CreateFile False,err:%d\n", GetLastError());
        return 0;
    }

    //
    //测试读写请求
    //
    bRet = WriteFile(hFile, "I love U", 8, &dwRet, NULL);

    if (!bRet) {
        printf("WriteFile Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    bRet = ReadFile(hFile, Buffer, 20, &dwRet, NULL);

    if (!bRet) {
        printf("ReadFile Failure,err:%d\n", GetLastError);
        CloseHandle(hFile);
        return 0;
    }

    printf("ReadFile Buffer: %s Len:%d\n", Buffer, dwRet);
    //
    //测试设备控制请求
    //
    bRet = DeviceIoControl(hFile, IOCTL_BUFFERED_IO, "testbuffer", 10, Buffer, 20, &dwRet, NULL);

    if (!bRet) {
        printf("DeviceIoControl Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    printf("DeviceIoControl IOCTL_BUFFERED_IO: %s Len:%d\n", Buffer, dwRet);
    bRet = DeviceIoControl(hFile, IOCTL_INDIRECT_IO, NULL, 0, Buffer, 20, &dwRet, NULL);

    if (!bRet) {
        printf("DeviceIoControl Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    printf("DeviceIoControl IOCTL_INDIRECT_IO: %s Len:%d\n", Buffer, dwRet);
    bRet = DeviceIoControl(hFile, IOCTL_NEITHER_IO, "hello world", 11, Buffer, 20, &dwRet, NULL);

    if (!bRet) {
        printf("DeviceIoControl Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    printf("DeviceIoControl IOCTL_NEITHER_IO: %s Len:%d\n", Buffer, dwRet);
    //
    //获取共享内存地址
    //
    bRet = DeviceIoControl(hFile, IOCTL_SHARE_MEMORY, NULL, 0, &pShareAddr, 4, &dwRet, NULL);

    if (!bRet) {
        printf("DeviceIoControl Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
    }

    printf("DeviceIoControl IOCTL_SHARE_MEMORY pShareAddr: 0x%08x  ShareMemory:%s\n",
           pShareAddr, pShareAddr);
    bRet = WriteFile(hFile, "I love U \0", 10, &dwRet, NULL);

    if (!bRet) {
        printf("WriteFile Failure,err:%d\n", GetLastError());
        CloseHandle(hFile);
        return 0;
    }

    printf("ShareMemory: %s\n", pShareAddr);
    CloseHandle(hFile);
    return 0;
}