
// AProtectView.cpp : CMyAProtectView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "A-Protect.h"
#endif

#include "A-ProtectDoc.h"
#include "A-ProtectView.h"
#include "AboutDlg.h"
#include "SubModule.h"
#include "uninstall360.h"
#include "CProcessSearch.h"
#include "SnifferSetting.h"
#include "ProtectSetting.h"
#include "StackThread.h"
#include "LookUpKernelData.h"
#include "SelectKernelModuleHook.h"
#include "SelectAnyModule.h"

#include <DbgHelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern char lpOpenUrl[260];
BOOL TrustQuery = FALSE;  //数字签名检查
BOOL bIsChecking = FALSE; //当前的检查状态
HWND MainhWnd;

char *lpDebugHelpMd5 = "4003E34416EBD25E4C115D49DC15E1A7";
char *lpSymsrvMd5 = "39572DED651B59A792B3F0C82603BF9E";
char *lpWinspApi = "C4B470269324517EE838789C7CF5E606";

BOOL bIsPhysicalCheck = FALSE;  //一键体检
WCHAR PhysicalFile[260];
HANDLE g_hThreadEvent;
BOOL bIsNtosOrSelect = FALSE;
BOOL bIsCheckFile = TRUE;
BOOL bIsStopHookScan = FALSE;
BOOL bIsProcMD5Check = FALSE;   //是否使用MD5扫描 ，FALSE 为不使用
BOOL bIsStartSrvName = FALSE;
BOOL bIsRunAndTrust = FALSE;

void SaveTitleFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);
DWORD _stdcall PhysicalThread(CMyList *m_ListCtrl);
void DllModuleInfoForPhysical(CMyList *m_list);
BOOL Install(HWND hwndDlg);
BOOL WINAPI EnableDebugPriv(LPCTSTR name);//提提权函数
BOOL InstallDepthServicesScan(CHAR * serviceName);
BOOL UninstallDepthServicesScan(CHAR * serviceName);
VOID QueryInternetInfo(HWND hWnd);
BOOL IsWindows7();
BOOL QueryUserAgent(HKEY HKey,char *lpSubKey,char *lpValueName,char *OutBuffer);
VOID GetFileMd5Hash(char *lpszDLLPath,char *lpszMd5);
LPSTR ExtractFilePath(LPSTR lpcFullFileName);
void LoadNtkrnlSym(void);

WCHAR lpwzEthread[50] = {0};

extern BOOL bIsEyeOpen; //鹰眼~
//*********************************************************
//SSDT
//*********************************************************
#define   SSDT_MAX_COLUMN 8
wchar_t	SSDTStr[SSDT_MAX_COLUMN][260]  = {_T("ID"),	_T("当前地址"),_T("原始地址"),	_T("函数名"),	_T("内核模块"),		
	_T("模块基址"),	_T("模块大小"),	_T("Hook类型")};										 
int		   SSDTWidth[SSDT_MAX_COLUMN]= {40,80,80, 100, 180, 80, 70, 70};
VOID QuerySSDTHook(HWND m_hWnd,ULONG ID,int IntType,CMyList *m_list);
VOID UnHookSSDT(HWND m_hWnd,CMyList *m_list);
VOID UnHookSSDTAll(HWND m_hWnd,CMyList *m_list);
VOID CopySSDTDataToClipboard(HWND hWnd,CMyList *m_list);
//*********************************************************
//ShadowSSDT
//*********************************************************
#define	  SHADOW_MAX_COLUMN	8

wchar_t	ShadowSSDTStr[SHADOW_MAX_COLUMN][260]  = {_T("ID"),	_T("当前地址"),_T("原始地址"),	_T("函数名"),	_T("内核模块"),		
	_T("模块基址"),	_T("模块大小"),	_T("Hook类型")};							 
int		ShadowSSDTWidth[SHADOW_MAX_COLUMN]=	{40,80,80, 100, 180, 80, 70, 70};
VOID QueryShadowSSDTHook(HWND m_hWnd,ULONG ID,int IntType,CMyList *m_list);
VOID UnHookShadowSSDT(HWND m_hWnd,CMyList *m_list);
VOID UnHookShadowSSDTAll(HWND m_hWnd,CMyList *m_list);
VOID CopyShadowSSDTDataToClipboard(HWND hWnd,CMyList *m_list);
//*********************************************************
//KernelHook
//*********************************************************
#define   KERNEL_HOOK_MAX_COLUMN 10

wchar_t	HookStr[KERNEL_HOOK_MAX_COLUMN][260]  = {_T("被挂钩地址"),_T("原始地址"),		_T("函数"),	_T("hook跳转地址"),	_T("Hook模块"),	
	_T("模块基址"),	_T("模块大小"),_T("原始模块"),_T("模块基址"),	_T("Hook类型")};										 
int		HookWidth[KERNEL_HOOK_MAX_COLUMN]= {80,80,  100, 90, 180,  80, 80, 160,80,80};

VOID QueryKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
VOID UnHookKernelHookSelect(HWND m_hWnd,CMyList *m_list);
VOID ByPassKernelHook(HWND m_hWnd,CMyList *m_list);
void CopyKernelHookDataToClipboard(HWND m_hWnd,CMyList *m_list);
VOID QueryAllKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void SearchSelectModuleHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
//*********************************************************
//objecthook
//**********************************************************
#define	  OBJECT_HOOK_MAX_COLUMN	7

wchar_t	OBJStr[OBJECT_HOOK_MAX_COLUMN][260]  = {_T("当前函数地址"),	_T("原始函数地址"),			_T("函数名"),	_T("Object类型"),		
	_T("ObjectType地址"),	_T("当前函数地址所在模块"),	_T("Hook类型")};										 
int		OBJWidth[OBJECT_HOOK_MAX_COLUMN]=	{90, 90,110, 90, 100, 130, 90};
VOID QueryObjectHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***********************************************************************
//kernel module
//***********************************************************************
#define KERNEL_MODULE_MAX_COLUMN 8

wchar_t	ModuleStr[KERNEL_MODULE_MAX_COLUMN][260]  = {_T("基地址"),_T("大小"),_T("DriverObject"),	_T("驱动映像"),	_T("驱动路径"),		
	_T("服务"),	_T("MD5/数字签名")};

int		MWidth[KERNEL_MODULE_MAX_COLUMN]=	{80,80, 80, 80,150,70, 130};

VOID QueryKernelModule(HWND m_hWnd,ULONG ID,CMyList *m_list,int IntLookType);
void OnDumpmemorydatatofile(HWND hWnd,CMyList *m_list);
void CopyKernelModuleDataToClipboard(HWND m_hWnd,CMyList *m_list);
//***********************************************************************
//kernel thread
//***********************************************************************
#define KTHREAD_MAX_COLUMN 5
wchar_t	KernelThreadStr[KTHREAD_MAX_COLUMN][260]  = {_T("ETHREAD"),	_T("线程函数地址"),		
	_T("线程模块"),	_T("线程状态"),		_T("是否隐藏")};										 
int		KernelThreadWidth[KTHREAD_MAX_COLUMN]=	{100, 100, 280, 80, 70};
void QueryKernelThread(HWND m_hWnd,ULONG ID,CMyList *m_list);
void ClearKernelThreadLog(CMyList *m_list);
//***********************************************************************
//system thread
//***********************************************************************
#define STHREAD_MAX_COLUMN 10

wchar_t	SystemThreadStr[STHREAD_MAX_COLUMN][260]  = {_T("线程Id"),	_T("ETHREAD"),			_T("TEB"),	_T("线程入口"),		
	_T("HideDebug"),_T("优先级"),_T("切换次数"),_T("模块"),_T("线程状态"),_T("MD5/数字签名")};										 
int		SystemThreadWidth[STHREAD_MAX_COLUMN]=	{50,90, 70,90, 70,60,60,150,70,120};
void QuerySystemThread(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillSystemThread(HWND hWnd,ULONG ID,CMyList *m_list);
void SuspendThread(HWND hWnd,CMyList *m_list);
void ResumeThread(HWND hWnd,CMyList *m_list);
//***********************************************************************
//Dpc timer
//***********************************************************************
#define DPCTIMER_MAX_COLUMN 5

wchar_t	DpcTimerStr[DPCTIMER_MAX_COLUMN][260]  = {_T("KTIMER"),_T("KDPC"),_T("触发周期"),_T("函数入口"),_T("函数所在模块")};										 
int		DpcTimerWidth[DPCTIMER_MAX_COLUMN]=	{100, 90, 70, 90, 280};

void QueryDpcTimer(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillDpcTimer(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyDpcTimerDataToClipboard(HWND m_hWnd,CMyList *m_list);
//***********************************************************************
//SystemNotify
//***********************************************************************
#define SYSTEMNOTIFY_MAX_COLUMN 4

wchar_t	SystemNotifyStr[SYSTEMNOTIFY_MAX_COLUMN][260]  = {_T("回调入口"),_T("回调类型"),_T("对象"),_T("入口所在模块")};					 
int		SystemNotifyWidth[SYSTEMNOTIFY_MAX_COLUMN]=	{90, 90, 190, 250};

void QuerySystemNotify(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillSystemNotify(HWND hWnd,ULONG ID,CMyList *m_list);
void CopySystemNotifyDataToClipboard(HWND m_hWnd,CMyList *m_list);
//***********************************************************************
//WorkQueue
//***********************************************************************
#define WORKQUEUE_MAX_COLUMN 5

wchar_t	WorkQueueStr[WORKQUEUE_MAX_COLUMN][260]  = {_T("ETHREAD"),_T("队列线程类型"),_T("函数入口"),_T("暂停次数"),_T("入口所在模块")};					 
int		WorkQueueWidth[WORKQUEUE_MAX_COLUMN]=	{100,170, 90,80,220};

void QueryWorkQueue(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillWorkThread(HWND hWnd,ULONG ID,CMyList *m_list);
//暂停
void SuspendWorkQueueThread(HWND hWnd,ULONG ID,CMyList *m_list);

//恢复线程运行
void ResumeWorkQueueThread(HWND hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
//FilterDriver
//***************************************************************************
#define	  FILTER_MAX_COLUMN	5

wchar_t	GLStr[FILTER_MAX_COLUMN][260]  = {_T("类型"),	_T("驱动对象名"),			_T("设备"),	_T("宿主驱动对象名"),		
	_T("模块路径")};										 
int		GLWidth[FILTER_MAX_COLUMN]=	{90, 120, 90, 120, 250};
void QueryFilterDriver(HWND m_hWnd,ULONG ID,CMyList *m_list);
void DeleteSelectFilterDriver(HWND hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
//NTFS/FSD
//***************************************************************************

#define	  FSD_MAX_COLUMN	8

wchar_t	FSDStr[FSD_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		FSDWidth[FSD_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryNtfsFsdHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void FsdHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyFsdDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//tcpip
//***************************************************************************
#define  TCPIP_MAX_COLUMN  8

wchar_t	TcpStr[TCPIP_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		TcpWidth[TCPIP_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryTcpipHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void TcpipHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyTcpipDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//Nsiproxy
//***************************************************************************
#define  NSIPROXY_MAX_COLUMN  8

wchar_t	NsipStr[NSIPROXY_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		NsipWidth[NSIPROXY_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryNsipHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void NsipHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyNsipDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//Kbdclass
//***************************************************************************
#define  KBDCLASS_MAX_COLUMN  8

wchar_t	KbdclassStr[KBDCLASS_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		KbdclassWidth[KBDCLASS_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryKbdclassHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KbdclassHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyKbdclassDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//Mouclass
//***************************************************************************
#define  MOUCLASS_MAX_COLUMN  8

wchar_t	MouclassStr[MOUCLASS_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		MouclassWidth[MOUCLASS_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryMouclassHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void MouclassHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyMouclassDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//Aatapi
//***************************************************************************
#define  ATAPI_MAX_COLUMN  8

wchar_t	AtapiStr[ATAPI_MAX_COLUMN][260]  = {_T("例程序号"),	_T("函数名称"),			_T("当前地址"),	_T("原始地址"),		
	_T("当前函数所在模块"),_T("基址"),_T("大小"),	_T("Hook类型")};										 
int		AtapiWidth[ATAPI_MAX_COLUMN]=	{70, 120, 90, 90, 110, 80,60,70};
void QueryAtapiHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
void AtapiHookResetOne(HWND hWnd,ULONG ID,CMyList *m_list);
void CopyAtapiDataToClipboard(HWND hWnd,CMyList *m_list);
//***************************************************************************
//process
//***************************************************************************
#define	 PROCESS_MAX_COLUMN	7

wchar_t	ProcessStr[PROCESS_MAX_COLUMN][260]  = {_T("进程PID"),	_T("父进程PID"),			_T("进程名"),	_T("进程路径"),		
	_T("EPROCESS"),	_T("用户/内核 访问"),	_T("MD5/数字签名")};										 
int		Width[PROCESS_MAX_COLUMN]=	{60,60,100, 170, 90, 80,100};

VOID QuerySystemProcess(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillProcess(HWND hWnd,ULONG ID,CMyList *m_list);
void  KillProcessDeleteFile(HWND hWnd,ULONG ID,CMyList *m_list);
void ProcessVerify(HWND hWnd,CMyList *m_list,int Type);
void CopyProcessDataToClipboard(HWND hWnd,CMyList *m_list);
void CopyProcessMD5ToClipboard(HWND hWnd,CMyList *m_list);
void SuspendOrResumeProcess(CMyList *m_list,ULONG ulCommand);
//***************************************************************************
//services
//***************************************************************************
#define  SERVICES_MAX_COLUMN  7
wchar_t		    ServiceStr[SERVICES_MAX_COLUMN][260]  = {_T("状态"),			_T("启动类型"),_T("服务名"),		_T("描述"),		
	_T("映像路径"),	_T("动态链接库"),	_T("MD5/数字签名(动态链接库)")};										 
int				SrcWidth[SERVICES_MAX_COLUMN]=	{50, 70,80, 100,120, 120, 100};
VOID QueryServices(HWND m_hWnd,ULONG ID,CMyList *m_list,int IntType);
void GetDepthServicesList(HWND m_hWnd,ULONG ID,CMyList *m_list,int IntType);
void StartServices(HWND hWnd,CMyList *m_list);
void StopServices(HWND hWnd,CMyList *m_list);
void DeleteServices(HWND hWnd,CMyList *m_list);
void ManualServices(HWND hWnd,CMyList *m_list);
void AutoServices(HWND hWnd,CMyList *m_list);
void DisableServices(HWND hWnd,CMyList *m_list);
//***************************************************************************
//tcpview
//***************************************************************************
#define  TCPVIEW_MAX_COLUMN 7

wchar_t	TCPStr[TCPVIEW_MAX_COLUMN][260]  = {_T("协议"),	_T("本地地址"),			_T("远程地址"),	_T("连接状态"),		
	_T("PID"),	_T("进程路径"),	_T("MD5/数字签名")};										 
int		TCWidth[TCPVIEW_MAX_COLUMN]=	{50, 120, 120, 80, 40, 180, 90};
VOID Tcpview(HWND m_hWnd,ULONG ID,CMyList *m_list);
void TcpProKill(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
//HipsLog
//***************************************************************************
#define	 HIPSLOG_MAX_COLUMN	8

wchar_t	StealStr[HIPSLOG_MAX_COLUMN][260]  = {_T("进程ID"),_T("父进程ID"),_T("映像路径"),		
	_T("EPROCESS"),	_T("事件"),_T("事件详细内容"),_T("被创建进程/被修改服务"),_T("MD5/数字签名")};										 
int				SCWidth[HIPSLOG_MAX_COLUMN]=	{50, 60, 70, 70,70, 150,200,100};
VOID HipsLog(HWND m_hWnd,ULONG ID,CMyList *m_list,int Type);
void ClearListLog(CMyList *m_list);
void ConnectScan(HWND m_hWnd);
void SaveToFile(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
//数据监控
//***************************************************************************
#define	TCP_SNIFFER_MAX_COLUMN	3

wchar_t	TcpSnifferStr[TCP_SNIFFER_MAX_COLUMN][260]  = {_T("序号"),_T("自定义监控数据"),_T("描述")};										 
int				TcpSnifferWidth[TCP_SNIFFER_MAX_COLUMN]=	{70,200,400};
BOOL bIsOpenSniffer = FALSE;

BOOL TcpSnifferData(BOOL Type);
int StartTcpSniffer(char *argv,char *Buffer);

void DoCapture(CMyList *m_list);

void WriteSnifferInfo();
void ReadSnifferInfo(CMyList *m_list,char *lpData,char *lpDataDescription);
//***************************************************************************
//启动项
//***************************************************************************
#define	STARTUP_MAX_COLUMN	4

wchar_t	StartupStr[STARTUP_MAX_COLUMN][260]  = {_T("序号"),_T("名称"),_T("注册表路径"),_T("键值")};										 
int		StartupWidth[STARTUP_MAX_COLUMN]={70,100,350,200};

VOID QuerySystemStartup(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
//体检
//***************************************************************************
#define	PHYSICAL_MAX_COLUMN	2

wchar_t	PhysicalStr[PHYSICAL_MAX_COLUMN][260]  = {_T("当前项目"),_T("体检内容")};										 
int		PhysicalWidth[PHYSICAL_MAX_COLUMN]={100,600};

//***********************************************************************
//IO定时器
//***********************************************************************
#define IOTIMER_MAX_COLUMN 4

wchar_t	IoTimerStr[IOTIMER_MAX_COLUMN][260]  = {_T("DecviceObject"),_T("定时器入口"),_T("状态"),_T("入口所在模块")};					 
int		IoTimerWidth[IOTIMER_MAX_COLUMN]=	{100,100,80,380};

void QueryIoTimer(HWND m_hWnd,ULONG ID,CMyList *m_list);
void CopyIoTimerDataToClipboard(HWND m_hWnd,CMyList *m_list);
void IoTimerControl(HWND m_hWnd,ULONG ID,CMyList *m_list,ULONG ulCommand);
//***********************************************************************
//鹰眼分析-->
//1:就是把人家的体检报告加密，然后上传到服务器，然后用我来给他们的电脑体检（PC）
//2:人家上传bin，然后由我用A盾来分析这个bin的行为                         （BIN）
//3:保存人家的启动项信息，然后进行鹰眼分析（这个以后再做）
//***********************************************************************
#define EYE_MAX_COLUMN 4

wchar_t	EyeStr[EYE_MAX_COLUMN][260]  = {_T("Key"),_T("鹰眼分析状态"),_T("分析类型"),_T("鹰眼诊断建议")};					 
int		EyeWidth[EYE_MAX_COLUMN]=	{100,100,80,380};

void EyeNetWork(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***********************************************************************
//消息钩子
//***********************************************************************
#define MESSAGE_HOOK_MAX_COLUMN 7

wchar_t	MessageHookStr[MESSAGE_HOOK_MAX_COLUMN][260]  = {_T("句柄"),_T("钩子类型"),_T("钩子函数入口"),_T("所在进程"),_T("进程PID"),_T("线程PID"),_T("钩子所在模块")};					 
int		MessageHookWidth[MESSAGE_HOOK_MAX_COLUMN]=	{100,100,80,100,80,80,380};

VOID QueryMessageHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
//***************************************************************************
IMPLEMENT_DYNCREATE(CMyAProtectView, CFormView)

BEGIN_MESSAGE_MAP(CMyAProtectView, CFormView)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
// 	ON_WM_SYSCOMMAND()
// 	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMyAProtectView::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMyAProtectView::OnNMRClickList2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CMyAProtectView::OnLvnItemchangedList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnNMRClickTree1)

	ON_COMMAND(IDM_MATREP_EXPORT,OnSelectExport)
	ON_COMMAND(ID_About, &CMyAProtectView::OnAbout)
	ON_COMMAND(ID_Exit, &CMyAProtectView::OnClose)
	ON_COMMAND(IDC_OpenURL, &CMyAProtectView::OnOpenUrl)
	ON_COMMAND(ID_OpenKernelType, &CMyAProtectView::OnOpenKernelType)
	ON_COMMAND(ID_ShutdownKernelType, &CMyAProtectView::OnShutdownKernelType)
	//ON_COMMAND(ID_WindowsOverhead, &CMyAProtectView::OnWindowsOverhead)
	//ON_COMMAND(ID_CancelTheOverhead, &CMyAProtectView::OnCancelTheOverhead)
	ON_COMMAND(ID_ProtectProcess, &CMyAProtectView::OnProtectProcess)
	ON_COMMAND(ID_UnProtectProcess, &CMyAProtectView::OnUnProtectProcess)
	ON_COMMAND(ID_Booting, &CMyAProtectView::OnBooting)
	ON_COMMAND(ID_CancelBooting, &CMyAProtectView::OnCancelBooting)
	ON_COMMAND(ID_OpenSrcDownLoad, &CMyAProtectView::OnOpenSrcDownLoad)
	ON_COMMAND(ID_GetPDB, &CMyAProtectView::OnGetPDB)
	ON_COMMAND(ID_BSOD, &CMyAProtectView::OnKernelBSOD)
	

	ON_COMMAND(ID_SSDTHook, &CMyAProtectView::SSDTHook)
	ON_COMMAND(ID_SSDTAllHook, &CMyAProtectView::SSDTAllHook)
	ON_COMMAND(ID_UnHookSSDTHookSelect, &CMyAProtectView::UnHookSSDTHookSelect)
	ON_COMMAND(ID_UnHookSSDTHookAll, &CMyAProtectView::UnHookSSDTHookAll)
	ON_COMMAND(ID_CopySSDTDataToClipboard, &CMyAProtectView::OnCopySSDTDataToClipboard)

	ON_COMMAND(ID_ShadowSSDTHook, &CMyAProtectView::ShadowSSDTHook)
	ON_COMMAND(ID_ShadowSSDTAllHook, &CMyAProtectView::ShadowSSDTAllHook)
	ON_COMMAND(ID_UnHookShadowSSDTHookSelect, &CMyAProtectView::UnHookShadowSSDTHookSelect)
	ON_COMMAND(ID_UnHookShadowSSDTHookAll, &CMyAProtectView::UnHookShadowSSDTHookAll)
	ON_COMMAND(ID_CopyShadowSSDTDataToClipboard, &CMyAProtectView::OnCopyShadowSSDTDataToClipboard)

	ON_COMMAND(ID_KernelHook, &CMyAProtectView::OnKernelHook)
	ON_COMMAND(ID_UnHookKernelHookSelect, &CMyAProtectView::OnUnHookKernelHookSelect)
	ON_COMMAND(ID_ByPassKernelHook, &CMyAProtectView::OnByPassKernelHook)
	ON_COMMAND(ID_CopyKernelHookDataToClipboard, &CMyAProtectView::OnCopyKernelHookDataToClipboard)
	ON_COMMAND(ID_AllKernelHook, &CMyAProtectView::OnAllKernelHook)
	ON_COMMAND(ID_StopHookScan, &CMyAProtectView::OnStopHookScan)

	ON_COMMAND(ID_KernelModule, &CMyAProtectView::OnKernelModule)
	ON_COMMAND(ID_KernelModuleTrust, &CMyAProtectView::OnKernelModuleTrust)
	ON_COMMAND(ID_DumpMemoryDataToFile, &CMyAProtectView::OnDumpMemoryDataToFile)
	ON_COMMAND(ID_CopyKernelModuleDataToClipboard, &CMyAProtectView::OnCopyKernelModuleDataToClipboard)

	ON_COMMAND(ID_KernelThreadList, &CMyAProtectView::OnKernelThreadList)
	ON_COMMAND(ID_ClearKernelThreadLog, &CMyAProtectView::OnClearKernelThreadLog)

	ON_COMMAND(ID_SystemThreadList, &CMyAProtectView::OnSystemThreadList)
	ON_COMMAND(ID_KillSelectThread, &CMyAProtectView::OnKillSystemThread)
	ON_COMMAND(ID_SuspendSelectThread, &CMyAProtectView::OnSuspendThread)
	ON_COMMAND(ID_ResumeSelectThread, &CMyAProtectView::OnResumeThread)

	ON_COMMAND(ID_FilterDriverList, &CMyAProtectView::OnFilterDriverList)
	ON_COMMAND(ID_DeleteSelectFilterDriver, &CMyAProtectView::OnDeleteSelectFilterDriver)

	ON_COMMAND(ID_FsdHookList, &CMyAProtectView::OnFsdHookList)
	ON_COMMAND(ID_FsdHookResetOne, &CMyAProtectView::OnFsdHookResetOne)
	ON_COMMAND(ID_CopyFsdDataToClipboard, &CMyAProtectView::OnCopyFsdDataToClipboard)

	ON_COMMAND(ID_TcpipHookList, &CMyAProtectView::OnTcpipHookList)
	ON_COMMAND(ID_TcpipHookResetOne, &CMyAProtectView::OnTcpipHookResetOne)
	ON_COMMAND(ID_CopyTcpipDataToClipboard, &CMyAProtectView::OnCopyTcpipDataToClipboard)

	ON_COMMAND(ID_NsipHookList, &CMyAProtectView::OnNsipHookList)
	ON_COMMAND(ID_NsipHookResetOne, &CMyAProtectView::OnNsipHookResetOne)
	ON_COMMAND(ID_CopyNsipDataToClipboard, &CMyAProtectView::OnCopyNsipDataToClipboard)

	ON_COMMAND(ID_KbdclassHookList, &CMyAProtectView::OnKbdclassHookList)
	ON_COMMAND(ID_KbdclassHookResetOne, &CMyAProtectView::OnKbdclassHookResetOne)
	ON_COMMAND(ID_CopyKbdclassDataToClipboard, &CMyAProtectView::OnCopyKbdclassDataToClipboard)

	ON_COMMAND(ID_MouclassHookList, &CMyAProtectView::OnMouclassHookList)
	ON_COMMAND(ID_MouclassHookResetOne, &CMyAProtectView::OnMouclassHookResetOne)
	ON_COMMAND(ID_CopyMouclassDataToClipboard, &CMyAProtectView::OnCopyMouclassDataToClipboard)

	ON_COMMAND(ID_AtapiHookList, &CMyAProtectView::OnAtapiHookList)
	ON_COMMAND(ID_AtapiHookResetOne, &CMyAProtectView::OnAtapiHookResetOne)
	ON_COMMAND(ID_CopyAtapiDataToClipboard, &CMyAProtectView::OnCopyAtapiDataToClipboard)

	ON_COMMAND(ID_DpcTimerList, &CMyAProtectView::OnDpcTimerList)
	ON_COMMAND(ID_KillDpcTimer, &CMyAProtectView::OnKillDpcTimer)
	ON_COMMAND(ID_CopyDpcTimerDataToClipboard, &CMyAProtectView::OnCopyDpcTimerDataToClipboard)

	ON_COMMAND(ID_SystemNotifyList, &CMyAProtectView::OnSystemNotifyList)
	ON_COMMAND(ID_KillSystemNotify, &CMyAProtectView::OnKillSystemNotify)
	ON_COMMAND(ID_CopySystemNotifyDataToClipboard, &CMyAProtectView::OnCopySystemNotifyDataToClipboard)

	ON_COMMAND(ID_Process, &CMyAProtectView::OnQueryProcess)
	ON_COMMAND(ID_ProcessTrust, &CMyAProtectView::OnQueryProcessTrust)
	ON_COMMAND(ID_ProcessModule, &CMyAProtectView::OnProcessmodule)
	ON_COMMAND(ID_ProcessModuleTrust, &CMyAProtectView::OnProcessmoduleTrust)
	ON_COMMAND(ID_ProcessThread, &CMyAProtectView::OnProcessThread)
	ON_COMMAND(ID_ProcessHandle, &CMyAProtectView::OnProcessHandle)
	
	ON_COMMAND(ID_KillProcess, &CMyAProtectView::OnKillProcess)
	ON_COMMAND(ID_KillProcessDeleteFile, &CMyAProtectView::OnKillProcessDeleteFile) 
	ON_COMMAND(ID_ProcessVerify, &CMyAProtectView::OnProcessVerify)
	ON_COMMAND(ID_TrustProcFile, &CMyAProtectView::OnTrustProcFile)

	ON_COMMAND(ID_CopyProcessDataToClipboard, &CMyAProtectView::OnCopyProcessDataToClipboard)
	ON_COMMAND(ID_CopyProcessMD5ToClipboard, &CMyAProtectView::OnCopyProcessMD5ToClipboard)
	
	ON_COMMAND(ID_QueryAllProcessModule,&CMyAProtectView::OnSearchDllModule)
	ON_COMMAND(ID_DLLModuleWithOutTrust,&CMyAProtectView::OnDLLModuleWithOutTrust)

	ON_COMMAND(ID_SearchProcessWithGoogle,&CMyAProtectView::OnSearchProcessWithGoogle)
	ON_COMMAND(ID_GetProcessFilePath,&CMyAProtectView::OnGetProcessFilePath)
	ON_COMMAND(ID_SuspendSelectProcess,&CMyAProtectView::OnSuspendProcess)
	ON_COMMAND(ID_ResumeSelectProcess,&CMyAProtectView::OnResumeProcess)


	ON_COMMAND(ID_GetServicesList, &CMyAProtectView::OnGetServicesList)
	ON_COMMAND(ID_GetServicesListByTrust, &CMyAProtectView::OnGetServicesListByTrust)
	ON_COMMAND(ID_RestartToGetServicesList, &CMyAProtectView::OnGetDepthServicesList)

	ON_COMMAND(ID_StartOnly, &CMyAProtectView::OnServicesStartOnly)
	ON_COMMAND(ID_UnTrustOnly, &CMyAProtectView::OnServicesUnTrustOnly)


	ON_COMMAND(ID_StartServices, &CMyAProtectView::OnStartServices)
	ON_COMMAND(ID_StopServices, &CMyAProtectView::OnStopServices)
	ON_COMMAND(ID_DeleteServices, &CMyAProtectView::OnDeleteServices)
	ON_COMMAND(ID_ManualServices, &CMyAProtectView::OnManualServices)
	ON_COMMAND(ID_AutoServices, &CMyAProtectView::OnAutoServices)
	ON_COMMAND(ID_DisableServices, &CMyAProtectView::OnDisableServices)



	ON_COMMAND(ID_Tcpview, &CMyAProtectView::OnMsgTcpView)
	ON_COMMAND(ID_KillTcpProcess, &CMyAProtectView::OnMsgTcpProKill)

	ON_COMMAND(ID_HipsLog, &CMyAProtectView::OnListLog)
	ON_COMMAND(ID_ClearListLog, &CMyAProtectView::OnClearListLog)
	ON_COMMAND(ID_ConnectScan, &CMyAProtectView::OnConnectScan)
	ON_COMMAND(ID_SaveToFile, &CMyAProtectView::OnSaveToFile)

	ON_BN_CLICKED(ID_SnifferTcpSetting, &CMyAProtectView::OnTCPSnifferSetting)

	ON_BN_CLICKED(ID_WrokQueue, &CMyAProtectView::OnBnWorkQueue)
	ON_BN_CLICKED(ID_KillWrokQueue, &CMyAProtectView::OnBnKillWorkQueue)
	ON_BN_CLICKED(ID_SuspendWrokQueue, &CMyAProtectView::OnSuspendWorkQueueThread)
	ON_BN_CLICKED(ID_ResumeWrokQueue, &CMyAProtectView::OnResumeWorkQueueThread)

	ON_BN_CLICKED(ID_IoTimerList, &CMyAProtectView::OnIoTimerList)
	ON_BN_CLICKED(ID_StopIoTimer, &CMyAProtectView::OnBnStopIoTimer)
	ON_BN_CLICKED(ID_StartIoTimer, &CMyAProtectView::OnBnStartIoTimer)
	ON_BN_CLICKED(ID_CopyIoTimerDataToClipboard, &CMyAProtectView::OnCopyIoTimerDataToClipboard)

	ON_BN_CLICKED(ID_MessageHookFunction, &CMyAProtectView::OnMessageHookFunction)
	

	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_SystemThreadStack, &CMyAProtectView::OnStackthread)
	ON_COMMAND(ID_DDCommand, &CMyAProtectView::OnBtnKernelData)
	ON_COMMAND(ID_OpenUrl, &CMyAProtectView::OnOpenurl)
	ON_COMMAND(ID_SelectModuleInlineHook, &CMyAProtectView::OnSelectmoduleinlinehook)
	ON_COMMAND(IDM_SSMODULE, &CMyAProtectView::OnSelectAnyModule)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CMyAProtectView::OnNMRClickTree1)
	END_MESSAGE_MAP()

// CMyAProtectView 构造/析构

CMyAProtectView::CMyAProtectView()
	: CFormView(CMyAProtectView::IDD)
{
	// TODO: 在此处添加构造代码
	m_case=0;
}

CMyAProtectView::~CMyAProtectView()
{
}

void CMyAProtectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}

BOOL CMyAProtectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////
//Function
DWORD WINAPI QuerySSDTHookFunction(CMyList *m_ListCtrl)
{
	QuerySSDTHook(MainhWnd,IDC_DebugStatus,0,m_ListCtrl);
	return 0;
}
DWORD WINAPI QuerySSDTAllHookFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QuerySSDTHook(MainhWnd,IDC_DebugStatus,1,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryShadowSSDTHookFunction(CMyList *m_ListCtrl)
{
	QueryShadowSSDTHook(MainhWnd,IDC_DebugStatus,0,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryShadowSSDTAllHookFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryShadowSSDTHook(MainhWnd,IDC_DebugStatus,1,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryKernelHookFunction(CMyList *m_ListCtrl)
{
	//是否停止hook扫描的开关，默认扫描
	bIsStopHookScan = FALSE;

	bIsChecking = TRUE;
	QueryKernelHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;

	bIsStopHookScan = TRUE;
	return 0;
}
DWORD WINAPI QueryAllKernelHookFunction(CMyList *m_ListCtrl)
{
	//是否停止hook扫描的开关，默认扫描
	bIsStopHookScan = FALSE;

	bIsChecking = TRUE;
	QueryAllKernelHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;

	bIsStopHookScan = TRUE;
	return 0;
}
DWORD WINAPI QuerySelectKernelHookFunction(CMyList *m_ListCtrl)
{
	//是否停止hook扫描的开关，默认扫描
	bIsStopHookScan = FALSE;

	bIsChecking = TRUE;
	SearchSelectModuleHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;

	bIsStopHookScan = TRUE;
	return 0;
}
DWORD WINAPI QueryObjectHookFunction(CMyList *m_ListCtrl)
{
	QueryObjectHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryKernelModuleFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryKernelModule(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryKernelModuleTrustFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	TrustQuery = TRUE;
	m_ListCtrl->DeleteAllItems();
	QueryKernelModule(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);
	TrustQuery = FALSE;
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryKernelThreadFunction(CMyList *m_ListCtrl)
{
	QueryKernelThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QuerySystemThreadFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QuerySystemThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryFilterDriverFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryFilterDriver(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryNtfsFsdHookFunction(CMyList *m_ListCtrl)
{
	QueryNtfsFsdHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryTcpipHookFunction(CMyList *m_ListCtrl)
{
	QueryTcpipHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryNsipHookFunction(CMyList *m_ListCtrl)
{
	QueryNsipHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QuerySystemProcessFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QuerySystemProcess(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QuerySystemProcessTrustFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	TrustQuery = TRUE;
	m_ListCtrl->DeleteAllItems();
	QuerySystemProcess(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	TrustQuery = FALSE;
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryServicesFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryServices(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI TcpviewFunction(CMyList *m_ListCtrl)
{
	Tcpview(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI HipsLogFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	TrustQuery = TRUE;
	HipsLog(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);
	TrustQuery = FALSE;
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryKbdclassHookFunction(CMyList *m_ListCtrl)
{
	QueryKbdclassHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryMouclassHookFunction(CMyList *m_ListCtrl)
{
	QueryMouclassHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryAtapiHookFunction(CMyList *m_ListCtrl)
{
	QueryAtapiHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryDpcTimerFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryDpcTimer(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QuerySystemNotifyFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QuerySystemNotify(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
//中间很多没有加
//下面是启动项
DWORD WINAPI QuerySystemStartupFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QuerySystemStartup(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI QueryWorkQueueFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	QueryWorkQueue(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI UnHookSSDTFunction(CMyList *m_ListCtrl)
{
	UnHookSSDT(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI UnHookSSDTAllFunction(CMyList *m_ListCtrl)
{
	UnHookSSDTAll(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopySSDTDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopySSDTDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI UnHookShadowSSDTFunction(CMyList *m_ListCtrl)
{
	UnHookShadowSSDT(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI UnHookShadowSSDTAllFunction(CMyList *m_ListCtrl)
{
	UnHookShadowSSDTAll(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyShadowSSDTDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyShadowSSDTDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI UnHookKernelHookSelectFunction(CMyList *m_ListCtrl)
{
	UnHookKernelHookSelect(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI ByPassKernelHookFunction(CMyList *m_ListCtrl)
{
	ByPassKernelHook(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyKernelHookDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyKernelHookDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI OnDumpmemorydatatofileFunction(CMyList *m_ListCtrl)
{
	OnDumpmemorydatatofile(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyKernelModuleDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyKernelModuleDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI ClearKernelThreadLogFunction(CMyList *m_ListCtrl)
{
	ClearKernelThreadLog(m_ListCtrl);
	return 0;
}
DWORD WINAPI KillSystemThreadFunction(CMyList *m_ListCtrl)
{
	KillSystemThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI DeleteSelectFilterDriverFunction(CMyList *m_ListCtrl)
{
	DeleteSelectFilterDriver(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI FsdHookResetOneFunction(CMyList *m_ListCtrl)
{
	FsdHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyFsdDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyFsdDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI TcpipHookResetOneFunction(CMyList *m_ListCtrl)
{
	TcpipHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyTcpipDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyTcpipDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI NsipHookResetOneFunction(CMyList *m_ListCtrl)
{
	NsipHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyNsipDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyNsipDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI TcpProKillFunction(CMyList *m_ListCtrl)
{
	TcpProKill(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI KillProcessFunction(CMyList *m_ListCtrl)
{
	KillProcess(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI KillProcessDeleteFileFunction(CMyList *m_ListCtrl)
{
	KillProcessDeleteFile(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI ProcessVerifyFunction(CMyList *m_ListCtrl)
{
	ProcessVerify(MainhWnd,m_ListCtrl,0);
	return 0;
}
DWORD WINAPI ProcessVerifyTrustFunction(CMyList *m_ListCtrl)
{
	ProcessVerify(MainhWnd,m_ListCtrl,1);
	return 0;
}
DWORD WINAPI CopyProcessDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyProcessDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryServicesTrustFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	TrustQuery = TRUE;
	m_ListCtrl->DeleteAllItems();
	QueryServices(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);
	TrustQuery = FALSE;
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI GetDepthServicesListFunction(CMyList *m_ListCtrl)
{
	GetDepthServicesList(MainhWnd,IDC_DebugStatus,m_ListCtrl,1);
	return 0;
}
DWORD WINAPI StartServicesFunction(CMyList *m_ListCtrl)
{
	StartServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI StopServicesFunction(CMyList *m_ListCtrl)
{
	StopServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI DeleteServicesFunction(CMyList *m_ListCtrl)
{
	DeleteServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI ManualServicesFunction(CMyList *m_ListCtrl)
{
	ManualServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI AutoServicesFunction(CMyList *m_ListCtrl)
{
	AutoServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI DisableServicesFunction(CMyList *m_ListCtrl)
{
	DisableServices(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI ClearListLogFunction(CMyList *m_ListCtrl)
{
	ClearListLog(m_ListCtrl);
	return 0;
}
DWORD WINAPI ConnectScanFunction()
{
	ConnectScan(MainhWnd);
	return 0;
}
DWORD WINAPI KbdclassHookResetOneFunction(CMyList *m_ListCtrl)
{
	KbdclassHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyKbdclassDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyKbdclassDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI MouclassHookResetOneFunction(CMyList *m_ListCtrl)
{
	MouclassHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyMouclassDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyMouclassDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI AtapiHookResetOneFunction(CMyList *m_ListCtrl)
{
	AtapiHookResetOne(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyAtapiDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyAtapiDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI KillDpcTimerFunction(CMyList *m_ListCtrl)
{
	KillDpcTimer(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyDpcTimerDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyDpcTimerDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI KillSystemNotifyFunction(CMyList *m_ListCtrl)
{
	KillSystemNotify(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopySystemNotifyDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopySystemNotifyDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
DWORD WINAPI SuspendWorkQueueThreadFunction(CMyList *m_ListCtrl)
{
	SuspendWorkQueueThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI ResumeWorkQueueThreadFunction(CMyList *m_ListCtrl)
{
	ResumeWorkQueueThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI KillWorkThreadFunction(CMyList *m_ListCtrl)
{
	KillWorkThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI QueryIoTimerFunction(CMyList *m_ListCtrl)
{
	bIsChecking = TRUE;
	m_ListCtrl->DeleteAllItems();
	QueryIoTimer(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	bIsChecking = FALSE;
	return 0;
}
DWORD WINAPI StopIoTimerFunction(CMyList *m_ListCtrl)
{
	IoTimerControl(MainhWnd,IDC_DebugStatus,m_ListCtrl,STOP_IO_TIMER);
	return 0;
}
DWORD WINAPI StartIoTimerFunction(CMyList *m_ListCtrl)
{
	IoTimerControl(MainhWnd,IDC_DebugStatus,m_ListCtrl,START_IO_TIMER);
	return 0;
}
DWORD WINAPI QueryMessageHookFunction(CMyList *m_ListCtrl)
{
	m_ListCtrl->DeleteAllItems();
	QueryMessageHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}
DWORD WINAPI CopyIoTimerDataToClipboardFunction(CMyList *m_ListCtrl)
{
	CopyIoTimerDataToClipboard(MainhWnd,m_ListCtrl);
	return 0;
}
void CMyAProtectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();  
    ResizeParentToFit();
	MainhWnd=m_hWnd;

	//以下为Tree图标处理
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();
	ImgList.Create(16,16,ILC_COLOR32,2,9);
	ImgList.Add(imgApp->LoadIconW(IDR_MyAProtectTYPE));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON1));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON2));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON3));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON4));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON5));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON6));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON7));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON8));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON9));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON10));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON11));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON12));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON13));
	//ImgList.Add(imgApp->LoadIconW(IDI_ICON14));
	//ImgList.Add(imgApp->LoadIconW(IDI_ICON15));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON16));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON17));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON18));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON19));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON20));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON21));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON22));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON23));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON24));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON25));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON26));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON28));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON30));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON34));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON40));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON45));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON48));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON49));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON50));
	ImgList.Add(imgApp->LoadIconW(IDI_ICON51));
	m_TreeCtrl.SetImageList(&ImgList,TVSIL_NORMAL);


	HTREEITEM hKernelHook=m_TreeCtrl.InsertItem(_T("内核Hook"),0,0,TVI_ROOT,TVI_LAST);//第一个数字为未选中时图片ID，第二个数字为选中时图片数字。
	HTREEITEM hKernelModule=m_TreeCtrl.InsertItem(_T("驱动模块"),1,1,TVI_ROOT,TVI_LAST);
	HTREEITEM hKernelThread=m_TreeCtrl.InsertItem(_T("内核信息"),2,2,TVI_ROOT,TVI_LAST);
	HTREEITEM hKernelFilterDriver=m_TreeCtrl.InsertItem(_T("过滤驱动"),3,3,TVI_ROOT,TVI_LAST);
	HTREEITEM hDispatch=m_TreeCtrl.InsertItem(_T("驱动例程"),4,4,TVI_ROOT,TVI_LAST);
	HTREEITEM hProcess=m_TreeCtrl.InsertItem(_T("系统进程"),5,5,TVI_ROOT,TVI_LAST);
	HTREEITEM hServices=m_TreeCtrl.InsertItem(_T("系统服务"),6,6,TVI_ROOT,TVI_LAST);
	HTREEITEM hTcpView=m_TreeCtrl.InsertItem(_T("网络连接"),7,7,TVI_ROOT,TVI_LAST);
	HTREEITEM hHips=m_TreeCtrl.InsertItem(_T("被动防御"),8,8,TVI_ROOT,TVI_LAST);

	HTREEITEM hTcpSniffer;
	//数据监控不支持win7
	if (!IsWindows7()){
		hTcpSniffer=m_TreeCtrl.InsertItem(_T("数据监控"),9,9,TVI_ROOT,TVI_LAST);
	}
	HTREEITEM hRunStart=m_TreeCtrl.InsertItem(_T("杂项检查"),10,10,TVI_ROOT,TVI_LAST);
	HTREEITEM hRing3Memory=m_TreeCtrl.InsertItem(_T("一键体检"),11,11,TVI_ROOT,TVI_LAST);
	HTREEITEM hEagleEye=m_TreeCtrl.InsertItem(_T("鹰眼分析"),13,13,TVI_ROOT,TVI_LAST);
	HTREEITEM hAProtectConfig=m_TreeCtrl.InsertItem(_T("手动配置"),12,12,TVI_ROOT,TVI_LAST);
	///////////////////////////////////////////////////////////////////////////////////
	HTREEITEM subSSDT=m_TreeCtrl.InsertItem(_T("SSDT"),16,16,hKernelHook,TVI_LAST);
	HTREEITEM subShadowSSDT=m_TreeCtrl.InsertItem(_T("ShadowSDT"),29,29,hKernelHook,TVI_LAST);
	HTREEITEM subNtosHook=m_TreeCtrl.InsertItem(_T("内核钩子"),21,21,hKernelHook,TVI_LAST);

	HTREEITEM subObjectHook=m_TreeCtrl.InsertItem(_T("ObjectHook"),28,28,hKernelHook,TVI_LAST);

	/////////////////////////////////////////////////////////////////////////
	HTREEITEM subKernelThread=m_TreeCtrl.InsertItem(_T("线程创建"),24,24,hKernelThread,TVI_LAST);
	HTREEITEM subSystemThread=m_TreeCtrl.InsertItem(_T("系统线程"),33,33,hKernelThread,TVI_LAST);
 	HTREEITEM subDpcTimer=m_TreeCtrl.InsertItem(_T("Dpc定时器"),26,26,hKernelThread,TVI_LAST);
	HTREEITEM subIoTimer=m_TreeCtrl.InsertItem(_T("IO定时器"),27,27,hKernelThread,TVI_LAST);
 	HTREEITEM subSystemProc=m_TreeCtrl.InsertItem(_T("系统回调"),23,23,hKernelThread,TVI_LAST);
	HTREEITEM subWorkQueue=m_TreeCtrl.InsertItem(_T("工作队列线程"),30,30,hKernelThread,TVI_LAST);
	////////////////////////////////////////////////////////////////////////
	HTREEITEM subDispatchNtfs=m_TreeCtrl.InsertItem(_T("Ntfs/Fsd"),15,15,hDispatch,TVI_LAST);

	if (IsWindows7())
	{
		HTREEITEM subDispatchNsiproxy=m_TreeCtrl.InsertItem(_T("Nsiproxy"),14,14,hDispatch,TVI_LAST);
	}else
		HTREEITEM subDispatchTcpip=m_TreeCtrl.InsertItem(_T("Tcpip"),17,17,hDispatch,TVI_LAST);

	HTREEITEM subDispatchKbdclass=m_TreeCtrl.InsertItem(_T("键盘Kbdclass"),31,31,hDispatch,TVI_LAST);
	HTREEITEM subDispatchMouclass=m_TreeCtrl.InsertItem(_T("鼠标Mouclass"),32,32,hDispatch,TVI_LAST);

	if (!IsWindows7())
		HTREEITEM subDispatchAtapi=m_TreeCtrl.InsertItem(_T("Atapi"),25,25,hDispatch,TVI_LAST);

	if (!IsWindows7()){
		HTREEITEM subOpenSniffer=m_TreeCtrl.InsertItem(_T("开启监控/刷新"),20,20,hTcpSniffer,TVI_LAST);
		HTREEITEM subOpenLog=m_TreeCtrl.InsertItem(_T("本机所有数据"),18,18,hTcpSniffer,TVI_LAST);
		HTREEITEM suhSnifferSetting=m_TreeCtrl.InsertItem(_T("监控设置"),19,19,hTcpSniffer,TVI_LAST);
	}
	HTREEITEM subRunStart =m_TreeCtrl.InsertItem(_T("启动项"),22,22,hRunStart,TVI_LAST);
	//HTREEITEM subMessageHook =m_TreeCtrl.InsertItem(_T("消息钩子"),22,22,hRunStart,TVI_LAST);
	//HTREEITEM suhFileCommand=m_TreeCtrl.InsertItem(_T("文件关联"),10,10,hRunStart,TVI_LAST);
	//HTREEITEM suhUrlProtocol=m_TreeCtrl.InsertItem(_T("URLProtocol"),10,10,hRunStart,TVI_LAST);
	///////////////////////////////////////////////////////////////////////
	LONG lStyle;
    lStyle = GetWindowLong(m_ListCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_ListCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

    DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	//选中某行使整行高亮（只适用与report风格的listctrl）LVS_EX_DOUBLEBUFFER双缓冲区，解决闪烁问题
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER;
    m_ListCtrl.SetExtendedStyle(dwStyle); //设置扩展风格
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);

	char lpszAProtectRunKey[100] = {0};
	memset(lpszAProtectRunKey,0,sizeof(lpszAProtectRunKey));
	QueryUserAgent(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run","A-Protect",lpszAProtectRunKey);
	if (strstr(lpszAProtectRunKey,"\\") == 0)
	{
		UninstallDepthServicesScan("A-Protect");
	}

	HKEY	hKey0;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\KeBugCheck",NULL,\
		KEY_WRITE,&hKey0) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey0);

		//删除，代表启动成功不蓝屏
		SHDeleteKeyA(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\KeBugCheck");
	}
	//*****************
	//检查当前目录下的文件是否合法
	//*****************
	char lpszCurrentDir[260] = {0};
	char lpszCurrentFile[260] = {0};
	char lpMd5[50] = {0};

	GetModuleFileNameA(NULL,lpszCurrentFile,sizeof(lpszCurrentFile));
	wsprintfA(lpszCurrentDir,"%s",ExtractFilePath(lpszCurrentFile));

	memset(lpszCurrentFile,0,sizeof(lpszCurrentFile));
	strcat(lpszCurrentFile,lpszCurrentDir);
	strcat(lpszCurrentFile,"\\dbghelp.dll");
	GetFileMd5Hash(lpszCurrentFile,lpMd5);
	if (_strcmpi(lpMd5,lpDebugHelpMd5) != 0){
		bIsCheckFile = FALSE;
		MessageBoxW(L"初始化文件错误！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}

	memset(lpszCurrentFile,0,sizeof(lpszCurrentFile));
	strcat(lpszCurrentFile,lpszCurrentDir);
	strcat(lpszCurrentFile,"\\symsrv.dll");
	GetFileMd5Hash(lpszCurrentFile,lpMd5);
	if (_strcmpi(lpMd5,lpSymsrvMd5) != 0){
		bIsCheckFile = FALSE;
		MessageBoxW(L"初始化文件错误！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
/*
	memset(lpszCurrentFile,0,sizeof(lpszCurrentFile));
	strcat(lpszCurrentFile,lpszCurrentDir);
	strcat(lpszCurrentFile,"\\winspapi.exe");
	GetFileMd5Hash(lpszCurrentFile,lpMd5);
	if (_strcmpi(lpMd5,lpWinspApi) != 0){
		bIsCheckFile = FALSE;
		MessageBoxW(L"初始化文件错误！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
*/
	//************************
	EnableDebugPriv(SE_DEBUG_NAME);
	Install(m_hWnd);

	HANDLE m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryInternetInfo,m_hWnd, 0,NULL);
	CloseHandle(m_hThread);

	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoadNtkrnlSym,0, 0,NULL));

	//窗口顶置
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	//默认开启进程保护自身
	DWORD dwReadByte;
	ReadFile((HANDLE)PROTECT_PROCESS,0,0,&dwReadByte,0);

	if (MessageBoxW(L"亲，初始化是否执行进程扫描？",L"A盾电脑防护",MB_ICONWARNING| MB_YESNO) == IDYES)
	{
		//初始化的时候，枚举进程
		for(int Index = 0; Index < PROCESS_MAX_COLUMN; Index++)
		{
			m_ListCtrl.InsertColumn(Index, ProcessStr[Index] ,LVCFMT_LEFT, Width[Index]);
		}
		m_case=12;
		//QuerySystemProcess(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySystemProcessFunction,&m_ListCtrl, 0,NULL);
	}
}



// CMyAProtectView 诊断

#ifdef _DEBUG
void CMyAProtectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyAProtectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMyAProtectDoc* CMyAProtectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyAProtectDoc)));
	return (CMyAProtectDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyAProtectView 消息处理程序

void CMyAProtectView::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hTree=m_TreeCtrl.GetSelectedItem();

	if(!m_TreeCtrl.ItemHasChildren(hTree))
	{
		if (!bIsCheckFile){
			MessageBoxW(L"初始化文件错误，A盾电脑防护的文件已被恶意更改\r\n\r\n请登陆http://www.3600safe.com/重新下载\r\n",L"A盾电脑防护",MB_ICONWARNING);
			return;
		}
		bIsStartSrvName = FALSE;
		bIsRunAndTrust = FALSE;
		if (wcslen(m_TreeCtrl.GetItemText(hTree)))
		{
			if (bIsPhysicalCheck &&
				TrustQuery)
			{
				MessageBoxW(L"亲，请耐心等待体检完毕...",L"A盾电脑防护",MB_ICONWARNING);
				return;
			}
			if (bIsChecking == TRUE)
			{
				MessageBoxW(L"当前检查正在进行，请稍后操作...",L"A盾电脑防护",MB_ICONWARNING);
				return;
			}
		}
		//清空listview
		m_ListCtrl.DeleteAllItems();

		for   (int Index=88;Index> -1;Index--)
		{
			m_ListCtrl.DeleteColumn(Index);
		}
		if(m_TreeCtrl.GetItemText(hTree)==_T("SSDT"))
		{
			for(int Index = 0; Index < SSDT_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, SSDTStr[Index] ,LVCFMT_CENTER, SSDTWidth[Index]);
			}
			m_case=1;
			//QuerySSDTHook(m_hWnd,IDC_DebugStatus,0,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySSDTHookFunction,&m_ListCtrl, 0,NULL);
		}else if(m_TreeCtrl.GetItemText(hTree)==_T("ShadowSDT"))
		{
			for(int Index = 0; Index < SHADOW_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, ShadowSSDTStr[Index] ,LVCFMT_CENTER, ShadowSSDTWidth[Index]);
			}
			m_case=2;
			//QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,0,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryShadowSSDTHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("内核钩子"))
		{
			for(int Index = 0; Index < KERNEL_HOOK_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, HookStr[Index] ,LVCFMT_CENTER, HookWidth[Index]);
			}
			m_case=3;
			//QueryKernelHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			bIsNtosOrSelect = FALSE;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryKernelHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("ObjectHook"))
		{
			for(int i = 0; i < OBJECT_HOOK_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,OBJStr[i], LVCFMT_LEFT,OBJWidth[i]);
			}
			m_case=4;
			//QueryObjectHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryObjectHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("驱动模块"))
		{
			for(int Index = 0; Index < KERNEL_MODULE_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, ModuleStr[Index] ,LVCFMT_LEFT, MWidth[Index]);
			}
			m_case=5;
			//QueryKernelModule(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryKernelModuleFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("线程创建"))
		{
			for(int i = 0; i < KTHREAD_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,KernelThreadStr[i], LVCFMT_LEFT,KernelThreadWidth[i]);
			}
			m_case=6;
			//QueryKernelThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryKernelThreadFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("系统线程"))
		{
			for(int i = 0; i < STHREAD_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,SystemThreadStr[i], LVCFMT_LEFT,SystemThreadWidth[i]);
			}
			m_case=7;
			//QuerySystemThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySystemThreadFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("过滤驱动"))
		{
			for(int i = 0; i < FILTER_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,GLStr[i], LVCFMT_LEFT,GLWidth[i]);
			}
			m_case=8;
			//QueryFilterDriver(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryFilterDriverFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("Ntfs/Fsd"))
		{
			for(int i = 0; i < FSD_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,FSDStr[i], LVCFMT_LEFT,FSDWidth[i]);
			}
			m_case=9;
			//QueryNtfsFsdHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryNtfsFsdHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("Tcpip"))
		{
			for(int Index = 0; Index < TCPIP_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, TcpStr[Index] ,LVCFMT_LEFT, TcpWidth[Index]);
			}
			m_case=10;
			//QueryTcpipHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryTcpipHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("Nsiproxy"))
		{
			for(int i = 0; i < NSIPROXY_MAX_COLUMN; i++)
			{
				m_ListCtrl.InsertColumn(i,NsipStr[i], LVCFMT_LEFT,NsipWidth[i]);
			}
			m_case=11;
			//QueryNsipHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryNsipHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("系统进程"))
		{
			for(int Index = 0; Index < PROCESS_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, ProcessStr[Index] ,LVCFMT_LEFT, Width[Index]);
			}
			m_case=12;

			//FALSE 为不使用MD5扫描
			bIsProcMD5Check = FALSE;

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySystemProcessFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("系统服务"))
		{
			for(int Index = 0; Index < SERVICES_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, ServiceStr[Index] ,LVCFMT_LEFT, SrcWidth[Index]);
			}
			m_case=13;
			//QueryServices(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryServicesFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("网络连接"))
		{
			for(int Index = 0; Index < TCPVIEW_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, TCPStr[Index] ,LVCFMT_LEFT, TCWidth[Index]);
			}
			m_case=14;
			//Tcpview(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TcpviewFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("被动防御"))
		{
			for(int Index = 0; Index < HIPSLOG_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, StealStr[Index] ,LVCFMT_LEFT, SCWidth[Index]);
			}
			m_case=15;
			//TrustQuery = TRUE;
			//HipsLog(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
			//TrustQuery = FALSE;
			CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HipsLogFunction,&m_ListCtrl,0,NULL));
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("键盘Kbdclass"))
		{
			for(int Index = 0; Index < KBDCLASS_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, KbdclassStr[Index] ,LVCFMT_CENTER, KbdclassWidth[Index]);
			}
			m_case=16;
			//QueryKbdclassHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryKbdclassHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("鼠标Mouclass"))
		{
			for(int Index = 0; Index < MOUCLASS_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, MouclassStr[Index] ,LVCFMT_CENTER, MouclassWidth[Index]);
			}
			m_case=17;
			//QueryMouclassHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryMouclassHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("Atapi"))
		{
			for(int Index = 0; Index < ATAPI_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, AtapiStr[Index] ,LVCFMT_CENTER, AtapiWidth[Index]);
			}
			m_case=18;
			//QueryAtapiHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryAtapiHookFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("Dpc定时器"))
		{
			for(int Index = 0; Index < DPCTIMER_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, DpcTimerStr[Index] ,LVCFMT_CENTER, DpcTimerWidth[Index]);
			}
			m_case=19;
			//QueryDpcTimer(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryDpcTimerFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("系统回调"))
		{
			for(int Index = 0; Index < SYSTEMNOTIFY_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, SystemNotifyStr[Index] ,LVCFMT_CENTER, SystemNotifyWidth[Index]);
			}
			m_case=20;
			//QuerySystemNotify(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySystemNotifyFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("开启监控/刷新"))
		{
			for(int Index = 0; Index < TCP_SNIFFER_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, TcpSnifferStr[Index] ,LVCFMT_CENTER, TcpSnifferWidth[Index]);
			}
			m_case=21;   //这是一个空的右键菜单

			//初始化的时候，如果记录文件不存在，则写入
			WriteSnifferInfo();

			//这里读取数据配置显示吧
			ReadSnifferInfo(&m_ListCtrl,0,0);

			if (!bIsOpenSniffer){

				//先停止并卸载
				TcpSnifferData(FALSE);

				//默认监控tcp
				StartTcpSniffer("tcp",0);

				//安装并启动
				TcpSnifferData(TRUE);
				//休眠下，不然可能不成功
				Sleep(5000);

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DoCapture,&m_ListCtrl, 0,0);

				bIsOpenSniffer = TRUE;
				MessageBoxW(L"数据包监控开启成功",L"A盾电脑防护",MB_ICONINFORMATION);
			}
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("本机所有数据"))
		{
			m_case=22;   //这是一个空的右键菜单

			if (!bIsOpenSniffer){
				MessageBoxW(L"未开启数据监控，无法查看！",L"A盾电脑防护",MB_ICONWARNING);
				return;
			}
			//这里读取数据配置显示吧
			ReadSnifferInfo(&m_ListCtrl,0,0);

			CHAR FilePath[260] = {0};

			//默认路径 c:\windows\A-Protect-TcpSniffer.txt
			memset(FilePath,0,sizeof(FilePath));
			GetWindowsDirectoryA(FilePath,sizeof(FilePath));
			strcat(FilePath,"\\A-Protect-TcpSniffer.txt");

			ShellExecuteA(0,0,FilePath,0,0,SW_SHOW);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("监控设置"))
		{
			m_case=23;   //这是一个空的右键菜单

			if (!bIsOpenSniffer){
				MessageBoxW(L"未开启数据监控，无法将设置写入配置文件！",L"A盾电脑防护",MB_ICONWARNING);
				return;
			}
			CSnifferSetting SniffSet;
			if(SniffSet.DoModal()==IDOK)
			{
				m_strData=SniffSet.m_strData;
				m_strDataDescription=SniffSet.m_strDataDescription;
			}
			if (wcslen(m_strData) < 2){
				return;
			}
			char lpData[260] = {0};
			char lpDataDescription[260] = {0};

			WideCharToMultiByte( CP_ACP,
				0,
				m_strData,
				-1,
				lpData,
				wcslen(m_strData)*2,
				NULL,
				NULL);
			WideCharToMultiByte( CP_ACP,
				0,
				m_strDataDescription,
				-1,
				lpDataDescription,
				wcslen(m_strDataDescription)*2,
				NULL,
				NULL);
			ReadSnifferInfo(0,lpData,lpDataDescription);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("手动配置"))
		{
			m_case=24;  //这是一个空的右键菜单

			CProtectSetting proset;
			proset.DoModal();
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("启动项"))
		{
			for(int Index = 0; Index < STARTUP_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, StartupStr[Index] ,LVCFMT_CENTER, StartupWidth[Index]);
			}
			m_case=25;
			//QuerySystemStartup(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QuerySystemStartupFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("工作队列线程"))
		{
			for(int Index = 0; Index < WORKQUEUE_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, WorkQueueStr[Index] ,LVCFMT_CENTER, WorkQueueWidth[Index]);
			}
			m_case=26;
			//QueryWorkQueue(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryWorkQueueFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("一键体检"))
		{
			for(int Index = 0; Index < PHYSICAL_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, PhysicalStr[Index] ,LVCFMT_CENTER, PhysicalWidth[Index]);
			}
			m_case=27;
			
			//搜索所有数据，没有通过数字签名的一律写入文件
			memset(PhysicalFile,0,sizeof(PhysicalFile));

			DWORD dwReadByte;
			//暂停不然无法因为保护着win7下无法打开对话框
			ReadFile((HANDLE)SUSPEND_PROTECT,0,0,&dwReadByte,0);
			CString			m_path;
			CFileDialog dlg( FALSE,L"txt",0, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,L"所有文件|*.*");
			dlg.m_ofn.lpstrTitle= L"A盾电脑防护 系统安全体检报告 保存";
			if ( dlg.DoModal() == IDOK )
			{
				m_path = dlg.GetPathName();
				wsprintfW(PhysicalFile,L"%ws",m_path);
				char lpszPhysical1[1024];
				char lpszPhysical[1024] = "***************************************************************\r\n"
					                      "[---A盾电脑防护 系统安全体检报告---]\r\n"
					                      "A盾电脑防护网站：http://www.3600safe.com/\r\n"
										  "A盾电脑防护0.2.5开源版本下载：http://www.3600safe.com/?post=102\r\n"
										  "*******************************************************************\r\n\r\n"
										  "报告生成时间：%d-%d-%d %d:%d\r\n"
										  "声明：A盾电脑防护生成的系统安全体检报告仅作为检查电脑系统安全的参考，不作为认定电脑系统安全的凭证\r\n"
										  "***************************************************************\r\n\r\n";

				SYSTEMTIME SystemTime;
				memset(lpszPhysical1,0,sizeof(lpszPhysical1));
				memset(&SystemTime,0,sizeof(SYSTEMTIME));
				GetLocalTime(&SystemTime);
				wsprintfA(lpszPhysical1,lpszPhysical,SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute);

				SaveTitleFile(lpszPhysical1,PhysicalFile);

				ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);
			}
			ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);

			if (GetFileAttributes(PhysicalFile) == -1)
			{
				AfxMessageBox(L"文件错误！");
				return;
			}

			bIsPhysicalCheck = TRUE;
			TrustQuery = TRUE;

			m_ListCtrl.DeleteAllItems();
			CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PhysicalThread,&m_ListCtrl,0,NULL));

		}else if(m_TreeCtrl.GetItemText(hTree)==_T("IO定时器"))
		{
			for(int Index = 0; Index < IOTIMER_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, IoTimerStr[Index] ,LVCFMT_CENTER, IoTimerWidth[Index]);
			}
			m_case=28;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryIoTimerFunction,&m_ListCtrl, 0,NULL);
		}
		else if(m_TreeCtrl.GetItemText(hTree)==_T("鹰眼分析"))
		{
			for(int Index = 0; Index < EYE_MAX_COLUMN; Index++)
			{
				m_ListCtrl.InsertColumn(Index, EyeStr[Index] ,LVCFMT_CENTER, EyeWidth[Index]);
			}
			m_case=29;
			
			if (bIsEyeOpen == EYEOPEN){

			}else if (bIsEyeOpen == EYECLOSE){
				AfxMessageBox(L"鹰眼分析功能暂停使用！");
				return;
			}else if (bIsEyeOpen == EYEUPDATE){
				AfxMessageBox(L"鹰眼分析功能云端升级中，稍后开放使用。");
				return;
			}
			else if (bIsEyeOpen == EYEUNKNOWN){
				AfxMessageBox(L"发生错误，鹰眼初始化失败，请确认您的电脑已经联网！");
				return;
			}else{
				AfxMessageBox(L"发生未知错误！");
				return;
			}
		}else if(m_TreeCtrl.GetItemText(hTree)==_T("消息钩子"))
		{
// 			for(int Index = 0; Index < MESSAGE_HOOK_MAX_COLUMN; Index++)
// 			{
// 				m_ListCtrl.InsertColumn(Index, MessageHookStr[Index] ,LVCFMT_CENTER, MessageHookWidth[Index]);
// 			}
// 			m_case=30;
// 			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryMessageHookFunction,&m_ListCtrl, 0,NULL);
		}
	}
}
DWORD _stdcall PhysicalThread(CMyList *m_ListCtrl)
{
	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对SSDT项目进行检查...");
	QuerySSDTHook(MainhWnd,IDC_DebugStatus,0,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对ShadowSSDT项目进行检查...");
	QueryShadowSSDTHook(MainhWnd,IDC_DebugStatus,0,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对NtosHook项目进行检查...");
	QueryKernelHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对ObjectHook项目进行检查...");
	QueryObjectHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对内核模块项目进行检查...");
	QueryKernelModule(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对系统线程项目进行检查...");
	QuerySystemThread(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Ntfs项目进行检查...");
	QueryNtfsFsdHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Tcpip项目进行检查...");
	QueryTcpipHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Nsiproxy项目进行检查...");
	QueryNsipHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Kbdclass项目进行检查...");
	QueryKbdclassHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Mouclass项目进行检查...");
	QueryMouclassHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对Atapi项目进行检查...");
	QueryAtapiHook(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对系统服务项目进行检查...");
	QueryServices(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对被动防御项目进行检查...");
	HipsLog(MainhWnd,IDC_DebugStatus,m_ListCtrl,0);

	//QuerySystemStartup(m_hWnd,IDC_DebugStatus,&m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"正在对系统进程项目进行检查...");

	//体检的时候使用MD5
	bIsProcMD5Check = TRUE;
	QuerySystemProcess(MainhWnd,IDC_DebugStatus,m_ListCtrl);

	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"未通过数字签名的DLL模块检查，可能会慢，请等待");

	DllModuleInfoForPhysical(m_ListCtrl);
// 	//初始化事件和关键段 自动置位,初始无触发的匿名事件
// 	g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL); 
// 	HANDLE handle = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DllModuleInfoForPhysical,0, 0, NULL);
// 	WaitForSingleObject(g_hThreadEvent, INFINITE); //等待线程返回

	TrustQuery = FALSE;
	bIsPhysicalCheck = FALSE;

	if (MessageBoxW(MainhWnd,L"体检完毕，是否打开体检报告？",L"A盾电脑防护",MB_ICONWARNING| MB_YESNO) == IDYES)
	{
		ShellExecuteW(0,0,PhysicalFile,0,0,SW_SHOW);
	}
	SetDlgItemTextW(MainhWnd,IDC_DebugStatus,L"体检完毕！");

	return 0;
}
void CMyAProtectView::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dwPos = GetMessagePos();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_iItem=pNMListView->iItem;//从0开始计数
    CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
	CMenu menu;

	if (bIsChecking == TRUE &&
		m_case != 3)  //Kernel Hook除外，因为KernelHook有停止按钮，在没有扫描完毕的时候，还是需要右键
	{
		MessageBoxW(L"当前检查正在进行，请耐心等待",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	switch (m_case)
	{
	case 1:
		menu.LoadMenu( IDR_SSDT_MENU );
		break;
	case 2:
		menu.LoadMenu(IDR_SHADOWSSDT_MENU);
		break;
	case 3:
		menu.LoadMenu(IDR_KERNELHOOK_MENU);
		break;
	case 5:
		menu.LoadMenu(IDR_KERNELMODULE_MENU);
		break;
	case 6:
		menu.LoadMenu(IDR_KERNELTHREAD_MENU);
		break;
	case 7:
		menu.LoadMenu(IDR_SYSTEMTHREAD_MENU);
		break;
	case 8:
		menu.LoadMenu(IDR_FILTERDRIVER_MENU);
		break;
	case 9:
		menu.LoadMenu(IDR_FSDHOOK_MENU);
		break; 
	case 10:
		menu.LoadMenu(IDR_TCPIPHOOK_MENU);
		break; 
	case 11:
		menu.LoadMenu(IDR_NSIPHOOK_MENU);
		break; 
	case 12:
		bIsProcMD5Check = TRUE;
		menu.LoadMenu(IDR_PROCESS_MENU);
		break; 
	case 13:
		menu.LoadMenu(IDR_SERVICES_MENU);
		break; 
	case 14:
		menu.LoadMenu(IDR_TCPVIEW_MENU);
		break; 
	case 15:
		menu.LoadMenu(IDR_HIPSLOG_MENU);
		break; 
	case 16:
		menu.LoadMenu(IDR_KBDCLASSHOOK_MENU);
		break; 
	case 17:
		menu.LoadMenu(IDR_MOUCLASSHOOK_MENU);
		break; 
	case 18:
		menu.LoadMenu(IDR_ATAPIHOOK_MENU);
		break; 
	case 19:
		menu.LoadMenu(IDR_DPCTIMER_MENU);
		break; 
	case 20:
		menu.LoadMenu(IDR_SYSTEMNOTIFY_MENU);
		break; 
	case 21:
		menu.LoadMenu(IDR_TCPSNIFFER_MENU);
		break; 
	case 26:
		menu.LoadMenu(IDR_WORK_QUEUE_MENU);
		break; 
	case 28:
		menu.LoadMenu(IDR_IOTIMER_MENU);
		break; 
	case 29:
		menu.LoadMenu(IDR_EYE_MENU);
		break; 
	case 30:
		menu.LoadMenu(IDR_MESSAGE_HOOK_MENU);
		break; 
	default:
		return;
	}
    CMenu* popup = menu.GetSubMenu(0);
    ASSERT( popup != NULL );
    popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
}
void CMyAProtectView::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
 	CAboutDlg	Dlg;
 	Dlg.DoModal();
}
void CMyAProtectView::OnOpenUrl()
{
	char szCommand[256] = {0};
	char lpszWinDir[256] = {0};
	char lpszSysDisk[10] = {0};

	if (strlen(lpOpenUrl) > 0)
	{
		// 		MessageBoxA(0,lpOpenUrl,0,0);
		// 		ShellExecuteA(0,0,lpOpenUrl,0,0,SW_SHOW);
		GetWindowsDirectoryA(lpszWinDir,sizeof(lpszWinDir));
		memcpy(lpszSysDisk,lpszWinDir,2);

		wsprintfA(szCommand,"%s\\Program Files\\Internet Explorer\\IEXPLORE.EXE %s",lpszSysDisk,lpOpenUrl);

		//MessageBoxA(0,szCommand,0,0);
		WinExec(szCommand,SW_SHOW);
	}
}

void CMyAProtectView::OnBooting()
{
	char lpszRun[256] = {0};
	char lpszModuleFile[256] = {0};
	DWORD dwReadByte;

	if (InstallDepthServicesScan("A-Protect"))
	{
		ReadFile((HANDLE)PROTECT_DRIVER_FILE,0,0,&dwReadByte,0);

		//设置开机启动，说明exe驻留，要保护进程
		ReadFile((HANDLE)PROTECT_PROCESS,0,0,&dwReadByte,0);

		//设置注册表run项启动
		GetModuleFileNameA(NULL,lpszModuleFile,sizeof(lpszModuleFile));
		wsprintfA(lpszRun,"reg add HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /v A-Protect /t REG_SZ /d \"%s\" /f",lpszModuleFile);
		WinExec(lpszRun,SW_HIDE);

		MessageBoxW(L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}
void CMyAProtectView::OnCancelBooting()
{
	DWORD dwReadByte;
	if (UninstallDepthServicesScan("A-Protect"))
	{
		ReadFile((HANDLE)UNPROTECT_DRIVER_FILE,0,0,&dwReadByte,0);

		ReadFile((HANDLE)UNPROTECT_PROCESS,0,0,&dwReadByte,0);

		SHDeleteValueA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run","A-Protect");  //删除启动run项
		MessageBoxW(L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}
void CMyAProtectView::OnClose()
{
	if (MessageBoxW(L"退出之后，“A盾电脑防护”将无法保护内核安全，同时无法防御木马对系统的攻击行为，确定要退出吗？", L"A盾电脑防护", MB_ICONINFORMATION|MB_YESNO) == IDYES)
	{
		DWORD dwReadByte;
		ReadFile((HANDLE)EXIT_PROCESS,0,0,&dwReadByte,0);
		Sleep(2000);
		exit(0);
		//ExitProcess(0);
	}
}
void CMyAProtectView::OnOpenKernelType()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwReadByte;
	ReadFile((HANDLE)KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);
	AfxMessageBox(L"开启\"内核安全模式\"成功!");
}
void CMyAProtectView::OnShutdownKernelType()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwReadByte;
	ReadFile((HANDLE)NO_KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);
	AfxMessageBox(L"关闭\"内核安全模式\"成功!");
}
void CMyAProtectView::OnOpenSrcDownLoad()
{
	ShellExecuteW(0,0,L"http://www.3600safe.com/3600safeOpenSource.rar",0,0,SW_SHOW);
}
void CMyAProtectView::OnKernelBSOD()
{
	DWORD dwReadByte;

	if (MessageBoxW(L"是否确定手动蓝屏？", L"A盾电脑防护", MB_ICONINFORMATION|MB_YESNO) == IDYES)
	{
		ReadFile((HANDLE)KERNEL_BSOD,0,0,&dwReadByte,0);
	}
}
///////////////////////////////////////////////////////
void CMyAProtectView::OnProtectProcess()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwReadByte;
	ReadFile((HANDLE)PROTECT_PROCESS,0,0,&dwReadByte,0);
	AfxMessageBox(L"开启“A盾电脑防护”自身进程保护成功!");
}
void CMyAProtectView::OnUnProtectProcess()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwReadByte;
	ReadFile((HANDLE)UNPROTECT_PROCESS,0,0,&dwReadByte,0);
	AfxMessageBox(L"关闭“A盾电脑防护”自身进程保护!");
}
//*********************************************************
//SSDT
//*********************************************************
void CMyAProtectView::SSDTHook()
{
	m_ListCtrl.DeleteAllItems();
	//QuerySSDTHook(m_hWnd,IDC_DebugStatus,0,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySSDTHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::SSDTAllHook()
{
	m_ListCtrl.DeleteAllItems();
	//QuerySSDTHook(m_hWnd,IDC_DebugStatus,1,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySSDTAllHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::UnHookSSDTHookSelect()
{
	//UnHookSSDT(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UnHookSSDTFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::UnHookSSDTHookAll()
{
	//UnHookSSDTAll(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UnHookSSDTAllFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopySSDTDataToClipboard()
{
	//CopySSDTDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopySSDTDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//*********************************************************
//ShadowSSDT
//*********************************************************
void CMyAProtectView::ShadowSSDTHook()
{
	m_ListCtrl.DeleteAllItems();
	//QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,0,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryShadowSSDTHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::ShadowSSDTAllHook()
{
	m_ListCtrl.DeleteAllItems();
	//QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,1,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryShadowSSDTAllHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::UnHookShadowSSDTHookSelect()
{
	//UnHookShadowSSDT(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UnHookShadowSSDTFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::UnHookShadowSSDTHookAll()
{
	//UnHookShadowSSDTAll(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UnHookShadowSSDTAllFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyShadowSSDTDataToClipboard()
{
	//CopyShadowSSDTDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyShadowSSDTDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//******************************************************************
//kernel hook
//******************************************************************
void CMyAProtectView::OnKernelHook()
{
	if (bIsChecking == TRUE)
	{
		MessageBoxW(L"当前检查正在进行，请耐心等待",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	m_ListCtrl.DeleteAllItems();
	bIsNtosOrSelect = FALSE;
	//QueryKernelHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryKernelHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnAllKernelHook()
{
	if (bIsChecking == TRUE)
	{
		MessageBoxW(L"当前检查正在进行，请耐心等待",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	m_ListCtrl.DeleteAllItems();
	bIsNtosOrSelect = TRUE;
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryAllKernelHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnUnHookKernelHookSelect()
{
	//UnHookKernelHookSelect(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UnHookKernelHookSelectFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnByPassKernelHook()
{
	//ByPassKernelHook(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ByPassKernelHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyKernelHookDataToClipboard()
{
	//CopyKernelHookDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyKernelHookDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//停止扫描
void CMyAProtectView::OnStopHookScan()
{
	if (!bIsStopHookScan){
		if (MessageBoxW(L"确定要停止当前扫描？",L"A盾电脑防护",MB_ICONWARNING| MB_YESNO) == IDYES)
		{
			bIsStopHookScan = TRUE;
		}
	}
}
//******************************************************************
//kernelmodule
//******************************************************************
void CMyAProtectView::OnKernelModule()
{
	m_ListCtrl.DeleteAllItems();
	//QueryKernelModule(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryKernelModuleFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKernelModuleTrust()
{
	/*TrustQuery = TRUE;
	m_ListCtrl.DeleteAllItems();
	QueryKernelModule(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
	TrustQuery = FALSE;*/
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryKernelModuleTrustFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnDumpMemoryDataToFile()
{
	//OnDumpmemorydatatofile(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnDumpmemorydatatofileFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyKernelModuleDataToClipboard()
{
	//CopyKernelModuleDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyKernelModuleDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//**********************************************************************
//kernel thread
//******************************************************************
void CMyAProtectView::OnKernelThreadList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryKernelThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryKernelThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnClearKernelThreadLog()
{
	//ClearKernelThreadLog(&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ClearKernelThreadLogFunction,&m_ListCtrl,0,NULL));
}
//***********************************************************************
//system thread
//******************************************************************
void CMyAProtectView::OnSystemThreadList()
{
	m_ListCtrl.DeleteAllItems();
	//QuerySystemThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKillSystemThread()
{
	//KillSystemThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillSystemThreadFunction,&m_ListCtrl,0,NULL));
};
void CMyAProtectView::OnSuspendThread()
{
	SuspendThread(m_hWnd,&m_ListCtrl);
	m_ListCtrl.DeleteAllItems();
	//QuerySystemThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnResumeThread()
{
	ResumeThread(m_hWnd,&m_ListCtrl);
	m_ListCtrl.DeleteAllItems();
	//QuerySystemThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemThreadFunction,&m_ListCtrl,0,NULL));
}
//***********************************************************************
//kernel filter driver
//******************************************************************
void CMyAProtectView::OnFilterDriverList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryFilterDriver(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryFilterDriverFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnDeleteSelectFilterDriver()
{
	//DeleteSelectFilterDriver(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DeleteSelectFilterDriverFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//ntfs fsd
//******************************************************************
void CMyAProtectView::OnFsdHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryNtfsFsdHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryNtfsFsdHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnFsdHookResetOne()
{
	//FsdHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FsdHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyFsdDataToClipboard()
{
	//CopyFsdDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyFsdDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//tcpip
//******************************************************************
void CMyAProtectView::OnTcpipHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryTcpipHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryTcpipHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnTcpipHookResetOne()
{
	//TcpipHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TcpipHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyTcpipDataToClipboard()
{
	//CopyTcpipDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyTcpipDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//nsiproxy
//******************************************************************
void CMyAProtectView::OnNsipHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryNsipHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryNsipHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnNsipHookResetOne()
{
	//NsipHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NsipHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyNsipDataToClipboard()
{
	//CopyNsipDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyNsipDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//process
//************************************************************************
void CMyAProtectView::OnQueryProcess()
{
	m_ListCtrl.DeleteAllItems();
	//QuerySystemProcess(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemProcessFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnQueryProcessTrust()
{
	/*TrustQuery = TRUE;
	m_ListCtrl.DeleteAllItems();
	QuerySystemProcess(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	TrustQuery = FALSE;*/
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemProcessTrustFunction,&m_ListCtrl,0,NULL));
}
WCHAR lpwzNum[50];  //全局变量
void CMyAProtectView::OnProcessmodule()
{
	CString PidNum;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中
	PidNum.Format(L"%s",m_ListCtrl.GetItemText(Item,0));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	CSubModule m_Module;
	m_Module.m_Subcase = 1;//区分进程模块、进程Hook、进程钩子等。
	m_Module.m_SubItem = 1;//m_iItem;//记录的右键的是哪一行
	m_Module.DoModal();
}
void CMyAProtectView::OnProcessmoduleTrust()
{
	CString PidNum;

	TrustQuery = TRUE;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中
	PidNum.Format(L"%s",m_ListCtrl.GetItemText(Item,0));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	CSubModule m_Module;
	m_Module.m_Subcase = 1;//区分进程模块、进程Hook、进程钩子等。
	m_Module.m_SubItem = 1;//m_iItem;//记录的右键的是哪一行
	m_Module.DoModal();

	TrustQuery = FALSE;
}
void CMyAProtectView::OnProcessHandle()
{
	CString PidNum;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中
	PidNum.Format(L"%s",m_ListCtrl.GetItemText(Item,0));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	CSubModule m_Module;
	m_Module.m_Subcase = 2;//区分进程模块、进程Hook、进程钩子等。
	m_Module.m_SubItem = 2;//m_iItem;//记录的右键的是哪一行
	m_Module.DoModal();
}
void CMyAProtectView::OnProcessThread()
{
	CString EProcess;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中
	EProcess.Format(L"%s",m_ListCtrl.GetItemText(Item,4));

	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,EProcess);

	CSubModule m_Module;
	m_Module.m_Subcase = 3;//区分进程模块、进程Hook、进程钩子等。
	m_Module.m_SubItem = 3;//m_iItem;//记录的右键的是哪一行

	m_Module.DoModal();
}
void CMyAProtectView::OnKillProcess()
{
	//KillProcess(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillProcessFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKillProcessDeleteFile()
{
	//KillProcessDeleteFile(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillProcessDeleteFileFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnProcessVerify()
{
	//ProcessVerify(m_hWnd,&m_ListCtrl,0);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ProcessVerifyFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnTrustProcFile()
{
	//ProcessVerify(m_hWnd,&m_ListCtrl,1);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ProcessVerifyTrustFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyProcessDataToClipboard()
{
	//CopyProcessDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyProcessDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyProcessMD5ToClipboard()
{
	CopyProcessMD5ToClipboard(m_hWnd,&m_ListCtrl);
}
void CMyAProtectView::OnSearchDllModule()
{
	//定义一个对象后，先设置对象的idFrom的值，然后再调用DoModal
	CCProcessSearch prodlg;
	prodlg.idFrom=1;  //通过ID来判断调用的是查找模块
	prodlg.DoModal();
}
void CMyAProtectView::OnDLLModuleWithOutTrust()
{
	//定义一个对象后，先设置对象的idFrom的值，然后再调用DoModal
	CCProcessSearch prodlg;
	prodlg.idFrom=2; //通过ID来判断调用的是查找没有通过数字签名的
	prodlg.DoModal();
}
void CMyAProtectView::OnSearchProcessWithGoogle()
{
	CString ProcName;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	ProcName.Format(L"%s",m_ListCtrl.GetItemText(Item,2));
	if (!wcslen(ProcName))
	{
		return;
	}
	WCHAR *lpURLFormat = L"http://www.google.com/search?q=%ws";
	WCHAR lpURL[260] = {0};

	wsprintfW(lpURL,lpURLFormat,ProcName);
	ShellExecuteW(0,0,lpURL,0,0,SW_SHOW);
}
void CMyAProtectView::OnGetProcessFilePath()
{
	CString ProcPath;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	ProcPath.Format(L"%s",m_ListCtrl.GetItemText(Item,3));
	if (!wcslen(ProcPath))
	{
		return;
	}
	//char lpProcPath[260] = {0};
	//WideCharToMultiByte (CP_OEMCP,NULL,ProcPath,-1,lpProcPath,wcslen(ProcPath)*2,NULL,FALSE);

	SHELLEXECUTEINFO ShExecInfo ={0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"properties";
	ShExecInfo.lpFile = ProcPath; //can be a file as well
	ShExecInfo.lpParameters = L"";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}
void CMyAProtectView::OnSuspendProcess()
{
	SuspendOrResumeProcess(&m_ListCtrl,SUSPEND_PROCESS);
	MessageBoxW(L"暂停进程运行操作成功！",L"A盾电脑防护",MB_ICONWARNING);
}
void CMyAProtectView::OnResumeProcess()
{
	SuspendOrResumeProcess(&m_ListCtrl,RESUME_PROCESS);
	MessageBoxW(L"恢复进程运行操作成功！",L"A盾电脑防护",MB_ICONWARNING);
}
//************************************************************************
//services
//************************************************************************
void CMyAProtectView::OnGetServicesList()
{
	bIsStartSrvName = FALSE;
	bIsRunAndTrust = FALSE;
	m_ListCtrl.DeleteAllItems();
	//QueryServices(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnGetServicesListByTrust()
{
	bIsStartSrvName = FALSE;
	bIsRunAndTrust = FALSE;

	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryServicesTrustFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnServicesStartOnly()
{
	bIsStartSrvName = TRUE;
	bIsRunAndTrust = FALSE;

	m_ListCtrl.DeleteAllItems();
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnServicesUnTrustOnly()
{
	bIsRunAndTrust = TRUE;
	bIsStartSrvName = TRUE;

	m_ListCtrl.DeleteAllItems();
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryServicesTrustFunction,&m_ListCtrl,0,NULL));
}

void CMyAProtectView::OnGetDepthServicesList()
{
	m_ListCtrl.DeleteAllItems();
	//GetDepthServicesList(m_hWnd,IDC_DebugStatus,&m_ListCtrl,1);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)GetDepthServicesListFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnStartServices()
{
	//StartServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StartServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnStopServices()
{
	//StopServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StopServices,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnDeleteServices()
{
	//DeleteServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DeleteServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnManualServices()
{
	//ManualServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ManualServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnAutoServices()
{
	//AutoServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AutoServicesFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnDisableServices()
{
	//DisableServices(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DisableServicesFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//tcpview
//************************************************************************
void CMyAProtectView::OnMsgTcpView()
{
	m_ListCtrl.DeleteAllItems();
	//Tcpview(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TcpviewFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnMsgTcpProKill()
{
	//TcpProKill(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TcpProKillFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//HipsLog
//************************************************************************
void CMyAProtectView::OnListLog()
{
	//TrustQuery = TRUE;
	m_ListCtrl.DeleteAllItems();
	//HipsLog(m_hWnd,IDC_DebugStatus,&m_ListCtrl,0);
	//TrustQuery = FALSE;
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HipsLogFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnClearListLog()
{
	//ClearListLog(&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ClearListLogFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnConnectScan()
{
	//ConnectScan(m_hWnd);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ConnectScanFunction,0,0,NULL));
}
void CMyAProtectView::OnSaveToFile()
{
	TrustQuery = TRUE;
	SaveToFile(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	TrustQuery = FALSE;
}
//************************************************************************
//Kbdclass
//******************************************************************
void CMyAProtectView::OnKbdclassHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryKbdclassHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryKbdclassHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKbdclassHookResetOne()
{
	//KbdclassHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KbdclassHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyKbdclassDataToClipboard()
{
	//CopyKbdclassDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyKbdclassDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//Mouclass
//******************************************************************
void CMyAProtectView::OnMouclassHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryMouclassHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryMouclassHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnMouclassHookResetOne()
{
	//MouclassHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MouclassHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyMouclassDataToClipboard()
{
	//CopyMouclassDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyMouclassDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//Atapi
//******************************************************************
void CMyAProtectView::OnAtapiHookList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryAtapiHook(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryAtapiHookFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnAtapiHookResetOne()
{
	//AtapiHookResetOne(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AtapiHookResetOneFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyAtapiDataToClipboard()
{
	//CopyAtapiDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyAtapiDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//DpcTimer
//******************************************************************
void CMyAProtectView::OnDpcTimerList()
{
	m_ListCtrl.DeleteAllItems();
	//QueryDpcTimer(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryDpcTimerFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKillDpcTimer()
{
	//KillDpcTimer(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillDpcTimerFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyDpcTimerDataToClipboard()
{
	//CopyDpcTimerDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyDpcTimerDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//SystemNotify
//******************************************************************
void CMyAProtectView::OnSystemNotifyList()
{
	m_ListCtrl.DeleteAllItems();
	//QuerySystemNotify(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySystemNotifyFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnKillSystemNotify()
{
	//KillSystemNotify(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillSystemNotifyFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopySystemNotifyDataToClipboard()
{
	//CopySystemNotifyDataToClipboard(m_hWnd,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopySystemNotifyDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnBnWorkQueue()
{
	m_ListCtrl.DeleteAllItems();
	//QueryWorkQueue(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryWorkQueueFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnBnKillWorkQueue()
{
	//KillWorkThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KillWorkThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnSuspendWorkQueueThread()
{
	//SuspendWorkQueueThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SuspendWorkQueueThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnResumeWorkQueueThread()
{
	//ResumeWorkQueueThread(m_hWnd,IDC_DebugStatus,&m_ListCtrl);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ResumeWorkQueueThreadFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnIoTimerList()
{
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryIoTimerFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnBnStopIoTimer()
{
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StopIoTimerFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnBnStartIoTimer()
{
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StartIoTimerFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnCopyIoTimerDataToClipboard()
{
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CopyIoTimerDataToClipboardFunction,&m_ListCtrl,0,NULL));
}
void CMyAProtectView::OnMessageHookFunction()
{
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QueryMessageHookFunction,&m_ListCtrl,0,NULL));
}
//************************************************************************
//数据监控，这里读取配置文件。
//************************************************************************
void CMyAProtectView::OnTCPSnifferSetting()
{

}
//************************************************************************
void CMyAProtectView::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////

void CMyAProtectView::OnStackthread()
{
	// TODO: 在此添加命令处理程序代码
	CString Ethread;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	Ethread.Format(L"%s",m_ListCtrl.GetItemText(Item,1));
	if (!wcslen(Ethread))
	{
		return;
	}
	memset(lpwzEthread,0,sizeof(lpwzEthread));
	wcscat(lpwzEthread,Ethread);
	CStackThread dlg;
	dlg.DoModal();
}


void CMyAProtectView::OnBtnKernelData()
{
	// TODO: 在此添加命令处理程序代码
	CLookUpKernelData KerData;
	KerData.DoModal();
}
void CMyAProtectView::OnGetPDB()
{
	MessageBox(L"发送邮件至 warehouse13@gmail.com\r\n\r\n邮件内容务必说明请求pdb的理由。\r\n",L"A盾电脑防护",MB_ICONWARNING);
}

void CMyAProtectView::OnOpenurl()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecuteW(0,0,L"http://www.3600safe.com/",0,0,SW_SHOW);
}

DWORD StringToHex(char* strSource);

VOID FixSelectModuleToKernel(ULONG ulModuleBase,WCHAR *ModulePath,char *lpszModulePath);

void CMyAProtectView::OnSelectmoduleinlinehook()
{
	/*
	// TODO: 在此添加命令处理程序代码
	CString str_ModuleBase;
	CString str_ModulePath;
	ULONG ulBase;

	WCHAR lpwzModuleBase[50] = {0};
	WCHAR lpwzModuleBase1[50] = {0};
	WCHAR lpwzModulePath[260] = {0};

	CHAR lpszModuleBase[50] = {0};
	CHAR lpszModulePath[260] = {0};

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_ListCtrl.GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	str_ModuleBase.Format(L"%s",m_ListCtrl.GetItemText(Item,0));
	str_ModulePath.Format(L"%s",m_ListCtrl.GetItemText(Item,4));

	wcscat(lpwzModulePath,str_ModulePath);
	wcscat(lpwzModuleBase1,str_ModuleBase);
	if (!wcslen(lpwzModuleBase1) ||
		!wcslen(lpwzModulePath))
	{
		return;
	}
	bIsChecking = TRUE;

	memcpy(lpwzModuleBase,lpwzModuleBase1+wcslen(L"0x"),wcslen(lpwzModuleBase1)*2-wcslen(L"0x"));

	WideCharToMultiByte( CP_ACP,
		0,
		lpwzModuleBase,
		-1,
		lpszModuleBase,
		wcslen(lpwzModuleBase)*2,
		NULL,
		NULL
		);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzModulePath,
		-1,
		lpszModulePath,
		wcslen(lpwzModulePath)*2,
		NULL,
		NULL
		);
	ulBase = StringToHex(lpszModuleBase);

	//MessageBoxA(0,lpszModulePath,lpszModuleBase,0);

	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"正在从微软服务器下载符号，请稍后...");
	SetDlgItemTextW(IDC_DebugStatus,lpwzTextOut);

	FixSelectModuleToKernel(ulBase,lpwzModulePath,lpszModulePath);

	bIsNtosOrSelect = TRUE;
	// TODO: 在此添加命令处理程序代码
	CSelectKernelModuleHook HookDlg;
	HookDlg.DoModal();

	SetDlgItemTextW(IDC_DebugStatus,L"操作完毕...");

	bIsChecking = FALSE;

	*/
}
void SearchHook();

void CMyAProtectView::OnSelectAnyModule()
{
	if (bIsChecking == TRUE)
	{
		MessageBoxW(L"当前检查正在进行，请耐心等待",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	// TODO: 在此添加命令处理程序代码
	CSelectAnyModule SelAnyModule;
	SelAnyModule.DoModal();

	m_ListCtrl.DeleteAllItems();
	bIsNtosOrSelect = TRUE;
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)QuerySelectKernelHookFunction,&m_ListCtrl,0,NULL));
}



void CMyAProtectView::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint cp;
	GetCursorPos(&cp);
	m_ListCtrl.ScreenToClient(&cp);
	int titem = m_ListCtrl.HitTest(cp, NULL);
	if(titem)
	{
		// 可以在此过滤点击树叶或树枝才弹出菜单，我没有限制
		HTREEITEM hTree=m_TreeCtrl.GetSelectedItem();
		if(m_TreeCtrl.GetItemText(hTree)==_T("系统线程"))
		{
			m_ListCtrl.SetItemCount(titem);
			m_ListCtrl.ClientToScreen(&cp);

			// 创建自定义菜单
			CMenu *pMenu = new CMenu();
			VERIFY(pMenu->CreatePopupMenu());
			pMenu->AppendMenu(MF_STRING,IDM_MATREP_EXPORT,L"查看线程堆栈");
			// 注意需要在资源文件Resource.h中定义菜单资源


			pMenu->TrackPopupMenu(TPM_LEFTALIGN,cp.x,cp.y,this);
			pMenu->DestroyMenu();
		}
	}

	*pResult = 0;
}

void CMyAProtectView::OnSelectExport()
{
	CStackThread	Dlg;
	Dlg.DoModal();
}