#include "stdafx.h"


//检测到的过滤驱动的信息
typedef struct _KERNEL_FILTERDRIVER_INFO{

	ULONG ulObjType;
	ULONG ulAttachDevice;
	WCHAR FileName[256];
	WCHAR FilePath[256];
	//宿主
	WCHAR HostFileName[256];
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

typedef struct _KERNEL_FILTERDRIVER{

	ULONG ulCount;
	KERNEL_FILTERDRIVER_INFO KernelFilterDriverInfo[1];

}KERNEL_FILTERDRIVER,*PKERNEL_FILTERDRIVER;

PKERNEL_FILTERDRIVER KernelFilterDriver;
CImageList DriverImg;//驱动图标
