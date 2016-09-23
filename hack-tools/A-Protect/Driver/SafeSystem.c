/*
写在前面的话：

最近YYY同学在发布某一个代码的时候，被XXX认为是在偷ZZZ的代码，然后XXX就很不爽的人肉了YYY同学的信息。这是事情的经过。
我想说的是，程序员那点事，谁写代码不是A来A去的。
XXX同学没A过吗？ZZZ同学就没有用过ctrl+c和ctrl+v吗？还发布人家的个人信息，这是侵犯隐私的行为，十几年的教育都还给老师了。

看来国内的环境真不适合free、open、share。

我很大方的承认，A盾的代码98%都是A的，现在代码放出来，赶紧来认领吧~~

PS：改版权的同学以后买套套都是穿孔的~

有技术问题发email：hack.x86.asm@gmail.com
or QQ:136618866

*/
#include "SafeSystem.h"

VOID DriverUnload(
	IN PDRIVER_OBJECT		DriverObject
	)
{
	KdPrint(("Driver Unload Called\n"));

}
VOID IsKernelBooting(IN PVOID Context)
{
	NTSTATUS status;
	PUCHAR KiFastCallEntry;
	ULONG EProcess;
	int i=0;
	ULONG ImageBase;

	if (PsGetProcessCount() <= 2)
		bKernelBooting = TRUE;
	else 
		goto _InitThread;  //系统没有刚刚启动，则跳过

	while (1)
	{
		//系统刚刚启动
		if (bKernelBooting){

			//等待初始化成功
			//初始化成功分为几个条件：
			//1：注册表初始化好了，可以访问、写入
			//2：文件系统初始化好了，可以访问文件、写入文件
			//3：用户空间进程环境已经初始化好了      <== 上面这两个判断有bug，这里判断才是比较准确的

			if (PsGetProcessCount() >= 3){
				break;
			}
		}
		WaitMicroSecond(88);
	}
	/*
		系统刚刚启动，执行到这里，足以说明是深度扫描。就开始检查
		如果是深度扫描，就开启蓝屏检测
		原理：写入KeBugCkeck值，ring3启动的时候删除这个键值，代表启动成功不蓝屏
		如果下次启动的时候发现KeBugCkeck存在，就说明启动不成功，就直接返回，不做任何操作
		如果发现这个键值，说明启动不成功，直接返回，不做任何hook
	*/
	if (IsRegKeyInSystem(L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\KeBugCheck")){
		/*

			驱动自启动不能调用PsTerminateSystemThread结束线程
			应该让线程自己返回

		*/
		return;
	}
	//写入一个值，在界面运行之后，删除，代表启动成功！
	//如果执行到这里，说明稳定运行不蓝屏！
	KeBugCheckCreateValueKey(L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\KeBugCheck");

_InitThread:
	_asm
	{
		pushad;
		mov ecx, 0x176;
		rdmsr;
		mov KiFastCallEntry, eax;
		popad;
	}
	/*
	  
	  防止重复加载
	  备注：如果注册开机启动，插入U盘就会第二次加载
	*/
	if (*KiFastCallEntry == 0xe9)
	{
		KdPrint(("Terminate System Thread"));

		/*

			驱动自启动不能调用PsTerminateSystemThread结束线程
			应该让线程自己返回

		*/
		return;
	}
	if (ReLoadNtos(PDriverObject,RetAddress) == STATUS_SUCCESS)
	{
		//添加驱动加载的被动防御，和驱动深度扫描
		PsSetLoadImageNotifyRoutine(ImageNotify);
		//*******************************************************************
		//test
		//*******************************************************************
		//ImageBase = 0xF83E2000;
		//IatHookCheck(&ImageBase);
		//MsGetMsgHookInfo();
		//*******************************************************************
		//---------------------------------------
		//demo，深度服务扫描隐藏
		//---------------------------------------
		if (bKernelBooting)
		{
			DepthServicesRegistry = (PSERVICESREGISTRY)ExAllocatePool(NonPagedPool,sizeof(SERVICESREGISTRY)*1024);
			if (DepthServicesRegistry)
			{
				memset(DepthServicesRegistry,0,sizeof(SERVICESREGISTRY)*1024);
				status = QueryServicesRegistry(DepthServicesRegistry);
				if (status == STATUS_SUCCESS)
				{
// 					Safe_CreateValueKey(
// 						L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\A-Protect",
// 						REG_SZ,
// 						L"QueryServicesRegistry",
// 						L"success");
				}
			}
		}
	}
	bKernelBooting = FALSE;
}
NTSTATUS DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath )
{
	ULONG ulSize;
	ULONG ulKeServiceDescriptorTable;
	int i = 0;
	HANDLE HThreadHandle;
	HANDLE ThreadHandle;

	DriverObject->DriverUnload = DriverUnload;

	PDriverObject = DriverObject;

	RetAddress=*(DWORD*)((DWORD)&DriverObject-4);

	ulMyDriverBase = DriverObject->DriverStart;
	ulMyDriverSize = DriverObject->DriverSize;

	DebugOn = FALSE;  //开启调式信息

	KdPrint(("//***************************************//\r\n"
	       	"//   A-Protect Anti-Rootkit Kernel Module  //\r\n"
			"//   Kernel Module Version LE 2012-0.4.3  //\r\n"
		     "//  website:http://www.3600safe.com       //\r\n"
	         "//***************************************//\r\n"));

	SystemEProcess = PsGetCurrentProcess();

	WinVersion = GetWindowsVersion();  //初始化系统版本
	if (WinVersion)
		KdPrint(("Init Windows version Success\r\n"));

	DepthServicesRegistry = NULL;
	//-----------------------------------------
	//创建一个系统线程做操作
	//-----------------------------------------
	if (PsCreateSystemThread(
		&HThreadHandle,
		0,
		NULL,
		NULL,
		NULL,
		IsKernelBooting,
		NULL) == STATUS_SUCCESS)
	{
		ZwClose(HThreadHandle);
	}
	return STATUS_SUCCESS;
}