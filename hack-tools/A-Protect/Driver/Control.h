#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "ntifs.h"
#include "stdlib.h"
#include "SSDT.h"
#include "ShadowSSDT.h"
#include "port.h"
#include "Tcpip.h"
#include "nsiproxy.h"
#include "Ntfs.h"
#include "deletefile.h"
#include "ProcessModule.h"
#include "KernelHookCheck.h"
#include "SysModule.h"
#include "Services.h"
#include "ObjectHookCheck.h"
#include "AntiInlineHook.h"
#include "KernelFilterDriver.h"
#include "SystemThread.h"
#include "KernelThread.h"
#include "kbdclass.h"
#include "Mouclass.h"
#include "Atapi.h"
#include "DpcTimer.h"
#include "SystemNotify.h"
#include "Startup.h"
#include "WorkQueue.h"
#include "LookUpKernelData.h"
#include "IoTimer.h"

#define NOHOOK 0;
#define SSDTHOOK 1
#define SSDTINLINEHOOK 2
#define INLINEHOOK 3

/*
 * 初始化
 */
#define SAFE_SYSTEM 0x18881111


#define LIST_SSDT   0x18881112                                //列举被挂钩的SSDT函数
#define LIST_SSDT_ALL    0x18881113                           //列举所有SSDT函数  
#define SET_ALL_SSDT 0x18881114                               //恢复所有SSDT函数HOOK
#define SET_ONE_SSDT 0x18881115                               //恢复单个SSDT函数hook
#define SET_INLINE_HOOK 0x18881116                            //恢复单个SSDT inline hook


#define LIST_TCPUDP 0x18881117                                //列举所有网络连接



#define KILL_PROCESS_BY_PID 0x18881118                        //从进程pid结束一个进程
#define LIST_PROCESS 0x18881119                               //列举所有进程
#define DELETE_FILE 0x18881120                                //删除一个文件


#define LIST_PROCESS_MODULE 0x18881121                        //列举进程DLL模块
#define INIT_PROCESS_LIST_PROCESS_MODULE 0x18881122           //在列举进程DLL模块的时候，必须要初始化某些参数


#define LIST_INLINEHOOK 0x18881123                            //枚举ntos*的hook，包括inline hook，eat hook
#define ANTI_INLINEHOOK 0x18881124                            //绕过ntos* inline hook


#define KERNEL_SAFE_MODULE 0x18881125                         //开启内核安全模式
#define NO_KERNEL_SAFE_MODULE 0x18881126                      //关闭内核安全模式


#define LIST_SYS_MODULE 0x18881127                           //列举内核模块

#define EXIT_PROCESS 0x18881128                              //退出3600safe的时候，必须要对内核做清理的命令


#define LIST_SERVICES 0x18881129                             //列举系统服务
#define CHANG_SERVICES_TYPE_1 0x18881130                     //服务手动
#define CHANG_SERVICES_TYPE_2 0x18881131                     //服务自动
#define CHANG_SERVICES_TYPE_3 0x18881132                     //服务禁用
#define LIST_DEPTH_SERVICES 0x18881133                       //深度服务扫描

#define LIST_LOG 0x18881134                                  //列举防御日志

#define INIT_DUMP_KERNEL_MODULE_MEMORY 0x18881135            //初始化dump内核模块
#define DUMP_KERNEL_MODULE_MEMORY 0x18881136                 //dump内核模块内存到文件


#define DIS_CREATE_PROCESS      0x18881137                   //禁止创建进程
#define CREATE_PROCESS          0x18881138                   //允许创建进程

#define DIS_WRITE_FILE      0x18881139                       //禁止创建文件
#define WRITE_FILE      0x18881140                           //允许创建文件 


#define DIS_LOAD_DRIVER      0x18881141                      //禁止加载驱动
#define LOAD_DRIVER      0x18881142                          //允许加载驱动


#define SHUT_DOWN_SYSTEM      0x18881143                     //强制环保重启系统


#define LIST_SHADOWSSDT       0x18881144                     //列举被挂钩的shadowSSDT函数
#define LIST_SHADOWSSDT_ALL       0x18881145                 //列举所有shadowSSDT函数

#define SET_ALL_SHADOWSSDT 0x18881146                        //恢复所有shadowSSDT hook
#define SET_ONE_SHADOWSSDT  0x18881147                       //恢复单个shadowSSDT hook  

#define SET_SHADOWSSDT_INLINE_HOOK 0x18881148                //恢复单个shadowSSDT inline hook

#define LIST_OBJECT_HOOK     0x18881149                      //李傕object hook

#define PROTECT_360SAFE 0x18881150                           //是否保护360  （已去掉此功能）
#define UNPROTECT_360SAFE 0x18881151                         //

#define LIST_FSD_HOOK     0x18881152                         //列举所有NTFS FSD HOOK
#define SET_FSD_HOOK      0x18881153                         //恢复NTFS FSD hook
#define INIT_SET_FSD_HOOK  0x18881154                        //恢复NTFS FSD inline hook之前的初始化工作

#define CLEAR_LIST_LOG  0x18881155                           //清空防御日志

#define LIST_KERNEL_FILTER_DRIVER   0x18881156               //列举过滤驱动
#define DELETE_KERNEL_FILTER_DRIVER   0x18881157             //摘除过滤驱动
#define INIT_KERNEL_FILTER_DRIVER     0x18881158             //摘除过滤驱动之前的初始化工作

#define ONLY_DELETE_FILE  0x18881159                         //直接删除文件，不Reload

#define LIST_TCPIP_HOOK         0x18881160                   //列举所有Tcpip.sys模块hook
#define SET_TCPIP_HOOK      0x18881161                       //恢复Tcpip.sys模块 hook
#define INIT_SET_TCPIP_HOOK  0x18881162                      //恢复Tcpip.sys模块 inline hook之前的初始化工作

#define LIST_NSIPROXY_HOOK         0x18881163                //列举所有Nsiproxy.sys模块hook
#define SET_NSIPROXY_HOOK      0x18881164                    //恢复Nsiproxy.sys模块 hook
#define INIT_SET_NSIPROXY_HOOK  0x18881165                   //恢复Nsiproxy.sys模块 inline hook之前的初始化工作

#define LIST_SYSTEM_THREAD   0x18881166                      //列举系统线程
#define KILL_SYSTEM_THREAD   0x18881167                      //结束系统线程

#define PROTECT_DRIVER_FILE  0x18881168                      //保护驱动文件(已去掉)
#define UNPROTECT_DRIVER_FILE  0x18881169

#define LIST_KERNEL_THREAD   0x18881170                      //列举内核线程
#define CLEAR_KERNEL_THREAD   0x18881171                     //清除内核线程日志

#define SET_EAT_HOOK          0x18881172                     //恢复eat hook

#define PROTECT_PROCESS       0x18881173                     //保护3600safe自身进程
#define UNPROTECT_PROCESS       0x18881174                   //不保护

#define DIS_RESET_SRV 0x18881175                             //禁止服务回写
#define RESET_SRV 0x18881176                                 //允许服务回写

#define KERNEL_THREAD 0x18881177                             //允许创建内核线程
#define DIS_KERNEL_THREAD 0x18881178                         //禁止创建内核线程 

#define RESUME_THREAD    0x18881179                          //恢复线程运行
#define SUSPEND_THREAD    0x18881180                         //暂停线程运行

#define LIST_KBDCLASS_HOOK         0x18881181                //列举所有kbdclass.sys模块hook
#define SET_KBDCLASS_HOOK      0x18881182                    //恢复kbdclass.sys模块 hook
#define INIT_SET_KBDCLASS_HOOK  0x18881183                   //恢复kbdclass.sys模块 inline hook之前的初始化工作


#define LIST_MOUCLASS_HOOK         0x18881184                //列举所有Mouclass.sys模块hook
#define SET_MOUCLASS_HOOK      0x18881185                    //恢复Mouclass.sys模块 hook
#define INIT_SET_MOUCLASS_HOOK  0x18881186                   //恢复Mouclass.sys模块 inline hook之前的初始化工作

#define LIST_ATAPI_HOOK         0x18881187                   //列举所有Atapi.sys模块hook
#define SET_ATAPI_HOOK      0x18881188                       //恢复Atapi.sys模块 hook
#define INIT_SET_ATAPI_HOOK  0x18881189                      //恢复Atapi.sys模块 inline hook之前的初始化工作

#define LIST_DPC_TIMER    0x18881190                         //枚举DPC定时器
#define KILL_DPC_TIMER    0x18881191                         //摘除DPC定时器

#define LIST_SYSTEM_NOTIFY    0x18881192                     //枚举系统回调
#define KILL_SYSTEM_NOTIFY    0x18881193                     //摘除系统回调
#define INIT_KILL_SYSTEM_NOTIFY 0x18881194                   //初始化摘除

#define INIT_PROCESS_THREAD   0x18881195                     //初始化进程EPROCESS
#define LIST_PROCESS_THREAD   0x18881196                     //获取进程线程

#define LIST_START_UP         0x18881197                     //启动项

#define LIST_WORKQUEUE        0x18881198                     //工作队列线程

#define INIT_PDB_KERNEL_INFO  0x18881199                     //从pdb里面读到的所有内核函数和名字

#define SUSPEND_PROCESS       0x18881200                     //暂停进程
#define RESUME_PROCESS        0x18881201                     //恢复进程运行

#define INIT_THREAD_STACK     0x18881202                     //初始化线程堆栈
#define LIST_THREAD_STACK     0x18881203                     //获取线程堆栈

#define INIT_KERNEL_DATA_BASE     0x18881204                 //初始要查看的起始地址
#define INIT_KERNEL_DATA_SIZE     0x18881205                 //初始化要查看的大小
#define LIST_KERNEL_DATA          0x18881206                 //开始查看咯

#define SUSPEND_PROTECT        0x18881207                    //暂停保护   
#define RESUME_PROTECT         0x18881208                    //恢复保护  

#define KERNEL_BSOD            0x18881209                   //手动蓝屏  

#define INIT_SELECT_MODULE_INLINE_HOOK  0x18881210          //初始化所选模块的inlinehook扫描 
#define LIST_SELECT_MODULE_INLINE_HOOK  0x18881211          //扫描所选模块的inlinehook扫描 

#define INIT_SET_SELECT_INLINE_HOOK 0x18881212              //初始化恢复inlinehook (原始地址)
#define INIT_SET_SELECT_INLINE_HOOK_1 0x18881213            //初始化恢复inlinehook (原始模块地址)
#define SET_SELECT_INLINE_HOOK      0x18881214              //开始恢复inlinehook
#define ANTI_SELECT_INLINE_HOOK     0x18881215              //绕过所选的inlinehook

#define SET_WINDOWS_HOOK            0x18881216             //允许全局钩子
#define DIS_SET_WINDOWS_HOOK        0x18881217             //拒绝全局钩子

#define INIT_DUMP_KERNEL_MODULE_MEMORY_1   0x18881218      //初始化dump大小

#define LIST_IO_TIMER               0x18881219             //枚举IO定时器
#define START_IO_TIMER               0x18881220            //启动IO定时器
#define STOP_IO_TIMER               0x18881221             //停止IO定时器

#define INIT_EAT_NUMBER          0x18881222                //初始化EAT的导入函数位置
#define INIT_EAT_REAL_ADDRESS          0x18881223          //初始化EAT的原始地址

#define DIS_DLL_FUCK                0x18881224             //关闭DLL挟持防护
#define DLL_FUCK                    0x18881225             //开启DLL挟持防护

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;
extern PDRIVER_OBJECT PDriverObject;
extern ULONG ulWin32kSize;

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;

extern BOOL bKernelSafeModule;  //内核安全模式

int ZwReadFileIndex;
int ZwTerminateProcessIndex;   //保护360进程

PVOID NtReadFileRet;
int NtReadFilePatchCodeLen;

PSSDTINFO SSDTInfo;
PTCPUDPINFO TCPUDPInfo;

BOOL bIsInitSuccess = FALSE;
BOOL bProtectProcess =  FALSE;    //默认不保护
BOOL bSSDTAll = FALSE;
BOOL bShadowSSDTAll = FALSE;
BOOL bProtect = FALSE;    //保护自己
BOOL bKernelSafeModule = FALSE;  //内核安全模式，默认关闭
BOOL bProtectDriverFile = FALSE;  //保护驱动文件，默认关闭，等注册开机启动的时候再来

BOOL bDisCreateProcess = TRUE;  //默认开启允许创建进程
BOOL bDisWriteFile = TRUE;  //默认开启允许创建文件
BOOL bDisLoadDriver = TRUE;  //默认允许加载驱动
BOOL bDisResetSrv = TRUE;    //默认允许设置服务
BOOL bDisKernelThread = TRUE;  //默认允许创建内核线程
BOOL bDisSetWindowsHook = TRUE;   //默认允许全局钩子
BOOL bDisDllFuck = TRUE;    //默认允许防护DLL挟持

BOOL bIsProtect360 = FALSE;  //默认不保护
BOOL bAntiDllInject = FALSE;  //拒绝dll注入
BOOL bInitWin32K = FALSE;
BOOL bPaused = FALSE;     //枚举进程的时候 要暂停下隐藏进程的枚举

ULONG ulInitEProcess;  //初始化ep，用来结束进程的
ULONG ulDumpKernelBase;  //初始化内核模块base
ULONG ulDumpKernelSize;

ULONG ulNumber;   // fsd hook 恢复的全局变量

extern BOOL DebugOn;
extern BOOL bKernelBooting;
extern ULONG ulWin32kBase;

PEPROCESS ProtectEProcess;  //A盾自己进程的eprocess
HANDLE ProtectProcessId;   //A盾自己进程的pid

PEPROCESS AttachGuiEProcess;
PEPROCESS CsrssEProcess;  //csrss的进程

PEPROCESS TempEProcess;

WCHAR lpwzFilter[100];
ULONG ulDeviceObject;

int IntNotify;

BOOL bKrnlPDBSuccess = FALSE;  //是否从ring3里获取了pdb
BOOL bModulePDBSuccess = FALSE;  //是否从ring3里获取了pdb

ULONG ulThread = 0;
ULONG ulInitRealFuncBase = 0;
/*ULONG ulReloadFuncBase = 0;*/
ULONG ulInitRealModuleBase = 0;
PVOID LookupBase = 0;
ULONG ulLookupSize = 0;

ULONG ulCallerFileSize = 6859776;

typedef struct _OBJECT_TYPE_INFORMATION { // Information Class 2
	UNICODE_STRING Name;
	ULONG ObjectCount;
	ULONG HandleCount;
	ULONG Reserved1[4];
	ULONG PeakObjectCount;
	ULONG PeakHandleCount;
	ULONG Reserved2[4];
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccess;
	UCHAR Unknown;
	BOOLEAN MaintainHandleDatabase;
	POOL_TYPE PoolType;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

//负责通信
typedef NTSTATUS (_stdcall * ZWREADFILE)(
	__in      HANDLE FileHandle,
	__in_opt  HANDLE Event,
	__in_opt  PIO_APC_ROUTINE ApcRoutine,
	__in_opt  PVOID ApcContext,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in      PVOID Buffer,
	__in      ULONG Length,
	__in_opt  PLARGE_INTEGER ByteOffset,
	__in_opt  PULONG Key
	);

typedef NTSTATUS (_stdcall * ZWTERMINATEPROCESS)(
	IN HANDLE  ProcessHandle,
	IN NTSTATUS  ExitStatus
	);

BOOL SystemCallEntryTableHook(
	PUNICODE_STRING FunctionName,
	int *Index,
	DWORD NewFuctionAddress
	);

BOOL PrintSSDT(PSSDTINFO SSDTInfo);
BOOL RestoreAllSSDTFunction(int IntType);
BOOL RestoreInlineHook(ULONG ulRealBase,ULONG ulRealModuleBase,ULONG ulReloadModuleBase);


BOOL PrintTcpIp(PTCPUDPINFO TCPUDPInfo);
BOOL PrintTcpIpInWin7(PTCPUDPINFO TCPUDPInfo);

NTSTATUS ReLoadTcpip(PDRIVER_OBJECT DriverObject,PTCPDISPATCHBAKUP TcpDispatchBakUp,int Type);
NTSTATUS ReLoadTcpipFree();
VOID SetTcpHook(ULONG ulNumber,ULONG ulRealDispatch);


NTSTATUS ReLoadNsiproxy(PDRIVER_OBJECT DriverObject,PNSIPROXYDISPATCHBAKUP NsiproxyDispatchBakUp,int Type);
NTSTATUS ReLoadNsiproxyFree();
VOID SetNsiproxyHook(ULONG ulNumber,ULONG ulRealDispatch);

BOOL GetNormalProcessList(PPROCESSINFO Info,PPROCESSINFO HideInfo);

BOOL KernelHookCheck(PINLINEHOOKINFO SelectModuleInlineHookInfo,int HookType);
VOID EnumKernelModule(PDRIVER_OBJECT PDriverObject,PSYSINFO SysModuleInfo);

VOID AntiInlineHook(ULONG ulRealBase,ULONG ulModuleBase,ULONG ulReloadModuleBase);

NTSTATUS ReLoadNtfs(PDRIVER_OBJECT DriverObject,PNTFSDISPATCHBAKUP NtfsDispatchBakUp,int IniType);
NTSTATUS ReLoadNtfsFree();
VOID SetFsdHook(ULONG ulNumber,ULONG ulRealDispatch);

VOID KillPro(ULONG ulPidOrEprocess);
BOOL Safe_CreateValueKey(PWCHAR SafeKey,ULONG Reg_Type,PWCHAR ValueName,PWCHAR Value);
BOOL DeleteRegKey(WCHAR *ServicesKey);
BOOL IsExitProcess(PEPROCESS Eprocess);
BOOL ProtectCode();  //自我保护
ULONG GetInheritedProcessPid(PEPROCESS Eprocess);
BOOL InitNetworkDefence();   //网络防御
BOOL InitWriteFile();     //禁止创建文件
BOOL InitZwSetValueKey();   //禁止服务回写/系统服务监视

NTSTATUS EnableDriverLoading();    //允许加载驱动
NTSTATUS DisEnableDriverLoading();  //禁止加载驱动

NTSTATUS  DebugWriteToFile(WCHAR *FileName,BYTE *buffer,ULONG ulSize);
NTSTATUS DumpMemory(PUCHAR SrcAddr, PUCHAR DstAddr, ULONG Size);

VOID ShadowSSDTHookCheck(PSHADOWSSDTINFO ShadowSSDTInfo);
BOOL RestoreAllShadowSSDTFunction(int IntType);
BOOL RestoreShadowInlineHook(ULONG ulNumber);

VOID IoFileObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);
VOID IoDeviceObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);
VOID IoDriverObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);
VOID CmpKeyObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);

KPROCESSOR_MODE KeGetPreviousMode();
VOID Fix360Hook(BOOL Is360);

NTSTATUS KernelFilterDriverEnum(PKERNEL_FILTERDRIVER KernelFilterDriver);
VOID ClearFilters(PWSTR lpwName,ULONG ulDeviceObject);

VOID QuerySystemThread(PSYSTEM_THREAD_INFO SystemThread,PEPROCESS EProcess);
//VOID QuerySystemThread(PSYSTEM_THREAD_INFO SystemThread);
BOOL KillThread(PETHREAD Thread);

BOOL InstallFileObejctHook();
BOOL InitKernelThreadData();
BOOL ReSetEatHook(int x,ULONG ulModuleBase,ULONG ulRealAddress);

NTSTATUS ResumeThread(PETHREAD Thread);
NTSTATUS SuspendThread(PETHREAD Thread);

NTSTATUS ReLoadKbdclass(PDRIVER_OBJECT DriverObject,PKBDCLASSDISPATCHBAKUP KbdclassDispatchBakUp,int Type);
VOID SetKbdclassHook(ULONG ulNumber,ULONG ulRealDispatch);

NTSTATUS ReLoadMouclass(PDRIVER_OBJECT DriverObject,PMOUCLASSDISPATCHBAKUP MouclassDispatchBakUp,int Type);
VOID SetMouclassHook(ULONG ulNumber,ULONG ulRealDispatch);

NTSTATUS ReLoadAtapi(PDRIVER_OBJECT DriverObject,PATAPIDISPATCHBAKUP AtapiDispatchBakUp,int Type);
VOID SetAtapiHook(ULONG ulNumber,ULONG ulRealDispatch);
NTSTATUS ReLoadAtapiFree();

ULONG QuerySystemNotify(PDRIVER_OBJECT DriverObj,PSYSTEM_NOTIFY SystemNotify);
VOID RemoveNotifyRoutine(ULONG ImageNotify,int Type);

VOID QueryStartup(PSTARTUP_INFO Startup);

VOID QueryWorkQueue(PWORKQUEUE WorkQueueThread);

VOID QueryIoTimer(PMyIoTimer IoTimer);
VOID IoTimerControl(PDEVICE_OBJECT DeviceObject,int Type);

NTSTATUS SuspendProcess(PEPROCESS EProcess);
NTSTATUS ResumeProcess(PEPROCESS EProcess);

NTSTATUS ReadThreadStack(PETHREAD Thread,PTHREAD_STACK StackOfThread);

VOID LookupKernelDataInfo(PVOID LookupBase,ULONG ulLookupSize,PLOOKUP_KERNEL_DATA LookupKernelData);

BOOL IsFileInSystem(WCHAR *lpwzFile);

BOOL SelectModuleHookCheck(PINLINEHOOKINFO SelectModuleInlineHookInfo);


BOOL MmIsAddressRangeValid(
	IN PVOID Address,
	IN ULONG Size
	);

#endif
