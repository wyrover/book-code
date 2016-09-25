#include <devioctl.h>

#ifndef _Win7x64Drv_H
#define _Win7x64Drv_H 1

#define DEVICE_NAME L"\\Device\\devWin7x64Drv" //Driver Name
#define LINK_NAME L"\\DosDevices\\Win7x64Drv"  //Link Name

#define IOCTL_BASE  0x800
#define MY_CTL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PsKillProcess64       MY_CTL_CODE(6)
#define IOCTL_PsSuspendProcess64    MY_CTL_CODE(7)
#define IOCTL_PsResumeProcess64     MY_CTL_CODE(8)

#endif