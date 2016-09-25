#include <devioctl.h>

#ifndef _Win7x64Drv_H
#define _Win7x64Drv_H 1

#define DEVICE_NAME L"\\Device\\devKRWProcess" //Driver Name
#define LINK_NAME L"\\DosDevices\\KRWProcess"  //Link Name

#define IOCTL_BASE  0x800
#define MY_CTL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_InputProcessId        MY_CTL_CODE(1)
#define IOCTL_InputBaseAddress      MY_CTL_CODE(2)
#define IOCTL_InputReadWriteLen     MY_CTL_CODE(3)
#define IOCTL_KReadProcessMemory    MY_CTL_CODE(4)
#define IOCTL_KWriteProcessMemory   MY_CTL_CODE(5)
#define IOCTL_MmKillProcess64       MY_CTL_CODE(6)  //PVASE

#endif