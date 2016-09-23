#include "StdAfx.h"
#include "DriverManager.h"
#include <winsvc.h>
#include <iostream>
using namespace std;

CDriverManager::CDriverManager(void)
{
}

CDriverManager::~CDriverManager(void)
{
}


// 装载NT驱动程序
//
BOOL CDriverManager::LoadNTDriver(PTCHAR lpszDriverName, PTCHAR lpszDriverPath)
{
	TCHAR szDriverImagePath[256];
	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);  // 得到完整的驱动路径

	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;  // SCM管理器的句柄
	SC_HANDLE hServiceDDK = NULL;  // NT驱动程序的服务句柄

	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  // 打开服务控制管理器

	if(hServiceMgr == NULL)  // OpenSCManager失败  
	{
		cout<<"OpenSCManager() Faild !"<<GetLastError()<<endl;
		bRet = FALSE;
		goto BeforeLeave;
	}
	else  // OpenSCManager成功
	{
		cout<<"OpenSCManager() ok !"<<endl;
	}

	// 创建驱动所对应的服务
	hServiceDDK = CreateService(hServiceMgr,
		lpszDriverName,  // 驱动程序的在注册表中的名字  
		lpszDriverName,  // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS,  // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,  // 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START,  // 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE,  // 注册表驱动程序的 ErrorControl 值  
		szDriverImagePath,  // 注册表驱动程序的 ImagePath 值  
		NULL,  NULL,  NULL,  NULL,  NULL);  

	DWORD dwRtn;
	if(hServiceDDK == NULL)  // 判断服务是否失败  
	{  
		dwRtn = GetLastError();
		if(dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)  
		{  
			cout<<"CrateService() Faild !"<<dwRtn<<endl;  // 由于其他原因创建服务失败
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{
			cout<<"CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! "<<endl;  // 服务创建失败，是由于服务已经创立过    
		}

		// 驱动程序已经加载，只需要打开  
		hServiceDDK = OpenService(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);  
		if(hServiceDDK == NULL)  // 如果打开服务也失败，则意味错误  
		{
			dwRtn = GetLastError();  
			cout<<"OpenService() Faild !"<<dwRtn<<endl;  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else 
		{
			cout<<"OpenService() ok !"<<endl;
		}
	}  
	else  
	{
		cout<<"CrateService() ok !"<<endl;
	}

	bRet = StartService(hServiceDDK, NULL, NULL);  // 开启此项服务   
	if(!bRet)  
	{  
		DWORD dwRtn = GetLastError();  
		if(dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING)  
		{  
			cout<<"StartService() Faild !"<<dwRtn<<endl;  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if(dwRtn == ERROR_IO_PENDING)   // 设备被挂住  
			{  
				cout<<"StartService() Faild ERROR_IO_PENDING !"<<endl; 
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  // 服务已经开启  
			{  
				cout<<"StartService() Faild ERROR_SERVICE_ALREADY_RUNNING !"<<endl;
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;

BeforeLeave:

	// 离开前关闭句柄
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}

	return bRet;
}


// 卸载NT驱动程序
//
BOOL CDriverManager::UnloadNTDriver(PTCHAR szSvrName)
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;  // SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;  // NT驱动程序的服务句柄
	SERVICE_STATUS SvrSta;
	
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);  // 打开SCM管理器  
	if(hServiceMgr == NULL)  // 带开SCM管理器失败  
	{
		cout<<"OpenSCManager() Faild !"<<GetLastError()<<endl;  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  // 带开SCM管理器成功  
	{
		cout<<"OpenSCManager() ok !"<<endl;  
	}
	
	hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);  // 打开驱动所对应的服务  
	if(hServiceDDK == NULL)  // 打开驱动所对应的服务失败  
	{
		cout<<"OpenService() Faild !"<<GetLastError()<<endl;  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{  
		cout<<"OpenService() ok !"<<endl;  
	}  
	// 停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。  
	if(!ControlService(hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta))  
	{  
		cout<<"ControlService() Faild !"<<GetLastError()<<endl;  
	}  
	else  // 打开驱动所对应的失败  
	{
		cout<<"ControlService() ok !"<<endl;  
	}

	// 动态卸载驱动程序  
	if(!DeleteService(hServiceDDK))  // 卸载失败  
	{
		cout<<"DeleteSrevice() Faild !"<<GetLastError()<<endl;  
	}  
	else  // 卸载成功  
	{  
		cout<<"DelServer:eleteSrevice() ok !"<<endl;  
	}  
	bRet = TRUE;

BeforeLeave:

	// 离开前关闭打开的句柄
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;	
}


