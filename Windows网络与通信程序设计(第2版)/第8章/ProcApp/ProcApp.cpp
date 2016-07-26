//////////////////////////////////////////////////////////
// ProcApp.cpp文件


#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include "ProcDrv.h"

int main()
{
	// 获取驱动驱动程序ProcDrv.sys的完整目录
	// 注意，您应该将ProcDrv工程编译产生的ProcDrv.sys文件复制到当前工程目录下
	char szDriverPath[256];
	char szLinkName[] = "slNTProcDrv";
	char* p;
	::GetFullPathName("ProcDrv.sys", 256, szDriverPath, &p);


	// 打开SCM管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCM == NULL)
	{
		printf(" 打开服务控制管理器失败，可能是因为您不拥有Administrator权限\n");
		return -1;
	}

	// 创建或打开服务
	SC_HANDLE hService = ::CreateService(hSCM, szLinkName, szLinkName, SERVICE_ALL_ACCESS, 
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
				szDriverPath, NULL, 0, NULL, NULL, NULL);
	if(hService == NULL)
	{
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			hService = ::OpenService(hSCM, szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	if(hService == NULL)
	{
		printf(" 创建服务出错！\n");
		return -1;
	}
	// 启动服务
	if(!::StartService(hService, 0, NULL))  // 这里调用DriverEntry例程
	{
		int nError = ::GetLastError();
		if(nError != ERROR_SERVICE_ALREADY_RUNNING)
		{	
			printf(" 启动服务出错！%d \n", nError);
			return -1;
		}
	}


	// 打开到驱动程序所控制设备的句柄
	char sz[256] = "";
	wsprintf(sz, "\\\\.\\%s", szLinkName);
	HANDLE hDriver = ::CreateFile(sz, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hDriver == INVALID_HANDLE_VALUE)
	{	
		printf(" 打开设备失败！ \n");
		return -1;
	}

	// 打开事件内核对象，等待驱动程序的事件通知
	HANDLE hProcessEvent = ::OpenEvent(SYNCHRONIZE, FALSE, "NTProcDrvProcessEvent");
	CALLBACK_INFO callbackInfo, callbackTemp = { 0 };
	while(::WaitForSingleObject(hProcessEvent, INFINITE) == WAIT_OBJECT_0)
	{
		// 向驱动程序发送控制代码
		DWORD nBytesReturn;
		BOOL bRet = ::DeviceIoControl(hDriver, IOCTL_NTPROCDRV_GET_PROCINFO, 
									NULL, 0, &callbackInfo, sizeof(callbackInfo), &nBytesReturn, NULL);
		if(bRet)
		{
			if(callbackInfo.hParentId != callbackTemp.hParentId 
						|| callbackInfo.hProcessId != callbackTemp.hProcessId 
							|| callbackInfo.bCreate != callbackTemp.bCreate)
			{
				if(callbackInfo.bCreate)
				{
					printf("	有进程被创建，PID: %d \n", callbackInfo.hProcessId);
				}
				else
				{
					printf("	有进程被终止，PID: %d \n", callbackInfo.hProcessId);
				}

				callbackTemp = callbackInfo;
			}
		//	break;   
		}
		else
		{
			printf(" 获取进程信息失败！\n");
			break;
		}
	
	}

	::CloseHandle(hDriver);

	// 等待服务完全停止运行
	SERVICE_STATUS ss;
	::ControlService(hService, SERVICE_CONTROL_STOP, &ss);
	// 从SCM数据库中删除服务
	::DeleteService(hService);		
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return 0;
}