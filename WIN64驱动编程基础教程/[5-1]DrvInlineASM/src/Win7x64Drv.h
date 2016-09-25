#include <devioctl.h>

#ifndef _Win7x64Drv_H
#define _Win7x64Drv_H 1
//============================================
#define DEVICE_NAME L"\\Device\\devWin64InlineASM" //Driver Name
#define LINK_NAME L"\\DosDevices\\Win64InlineASM"  //Link Name
//============================================
#define IOCTL_BASE  0x800
#define MY_CTL_CODE(i) \
    CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PauseThrdProtect  MY_CTL_CODE(1)
#define IOCTL_ResumeThrdProtect MY_CTL_CODE(2)
#define IOCTL_ProtectProcess    MY_CTL_CODE(3)
#define IOCTL_UnprotectProcess  MY_CTL_CODE(4)
#define IOCTL_ProtectThread     MY_CTL_CODE(5)
//============================================
#endif