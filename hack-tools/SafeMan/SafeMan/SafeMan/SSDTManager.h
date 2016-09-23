#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// SSDT结构
typedef struct _tagSSDT {
	PVOID pvSSDTBase;  // SSDT基地址
	PVOID pvServiceCounterTable;  // 指向另一个表，该表包含了每个服务表项被调用的次数
	ULONG ulNumberOfServices;  // 当前系统所支持的服务个数
	PVOID pvParamTableBase;  // 指向系统服务参数表，该表包含了每个服务所需的参数字节数
} SSDT, *PSSDT;


// SYSTEM_INFORMATION_CLASS中SystemModuleInformation的结构定义
typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
	HANDLE Section;
	PVOID MappedBase;
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT PathLength;  // 最后一个斜杠的位置
	CHAR ImageName[256];  // 完整的路径
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;


typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Count;
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S
{
	SystemBasicInformation,                // 00 Y N
	SystemProcessorInformation,            // 01 Y N
	SystemPerformanceInformation,          // 02 Y N
	SystemTimeOfDayInformation,            // 03 Y N
	SystemNotImplemented1,                 // 04 Y N
	SystemProcessesAndThreadsInformation,  // 05 Y N
	SystemCallCounts,                      // 06 Y N
	SystemConfigurationInformation,        // 07 Y N
	SystemProcessorTimes,                  // 08 Y N
	SystemGlobalFlag,                      // 09 Y Y
	SystemNotImplemented2,                 // 10 Y N
	SystemModuleInformation,               // 11 Y N
	SystemLockInformation,                 // 12 Y N
	SystemNotImplemented3,                 // 13 Y N
	SystemNotImplemented4,                 // 14 Y N
	SystemNotImplemented5,                 // 15 Y N
	SystemHandleInformation,               // 16 Y N
	SystemObjectInformation,               // 17 Y N
	SystemPagefileInformation,             // 18 Y N
	SystemInstructionEmulationCounts,      // 19 Y N
	SystemInvalidInfoClass1,               // 20
	SystemCacheInformation,                // 21 Y Y
	SystemPoolTagInformation,              // 22 Y N
	SystemProcessorStatistics,             // 23 Y N
	SystemDpcInformation,                  // 24 Y Y
	SystemNotImplemented6,                 // 25 Y N
	SystemLoadImage,                       // 26 N Y
	SystemUnloadImage,                     // 27 N Y
	SystemTimeAdjustment,                  // 28 Y Y
	SystemNotImplemented7,                 // 29 Y N
	SystemNotImplemented8,                 // 30 Y N
	SystemNotImplemented9,                 // 31 Y N
	SystemCrashDumpInformation,            // 32 Y N
	SystemExceptionInformation,            // 33 Y N
	SystemCrashDumpStateInformation,       // 34 Y Y/N
	SystemKernelDebuggerInformation,       // 35 Y N
	SystemContextSwitchInformation,        // 36 Y N
	SystemRegistryQuotaInformation,        // 37 Y Y
	SystemLoadAndCallImage,                // 38 N Y
	SystemPrioritySeparation,              // 39 N Y
	SystemNotImplemented10,                // 40 Y N
	SystemNotImplemented11,                // 41 Y N
	SystemInvalidInfoClass2,               // 42
	SystemInvalidInfoClass3,               // 43
	SystemTimeZoneInformation,             // 44 Y N
	SystemLookasideInformation,            // 45 Y N
	SystemSetTimeSlipEvent,                // 46 N Y
	SystemCreateSession,                   // 47 N Y
	SystemDeleteSession,                   // 48 N Y
	SystemInvalidInfoClass4,               // 49
	SystemRangeStartInformation,           // 50 Y N
	SystemVerifierInformation,             // 51 Y Y
	SystemAddVerifier,                     // 52 N Y
	SystemSessionProcessesInformation      // 53 Y N
} SYSTEM_INFORMATION_CLASS;


// mov eax, <ServiceIndex> 
// B8 <ServiceIndex> 
#pragma pack (push, 1)  // 变量对齐方式为1字节
typedef struct _tagSSDTEntry {
	BYTE byMov;  // 0xB8
	DWORD dwIndex;
} SSDTENTRY;
#pragma pack (pop)


#define MOV 0xB8


// IMAGE_BASE_RELOCATION.TypeOffset结构
typedef struct { 
	WORD offset:12;
	WORD type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;


// 重置SSDT信息
typedef struct _RESTORE_INFO {
	ULONG ulIndex;  // 服务号
	ULONG ulAddr;  // 服务函数的真实地址
} RESTORE_INFO, *PRESTORE_INFO;



// CSSDTManager 对话框

class CSSDTManager : public CDialog
{
	DECLARE_DYNAMIC(CSSDTManager)

public:
	CSSDTManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSSDTManager();

// 对话框数据
	enum { IDD = IDD_SSDTMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 初始化对话框
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSsdtRepairSelected();  // SSDT修复选中项按钮事件
	afx_msg void OnStnClickedStaticCheck();  // 全选按钮事件
	afx_msg void OnStnClickedStaticUncheck();  // 全不选按钮事件
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  // 设置鼠标样式
	afx_msg void OnNMCustomdrawListSsdt(NMHDR *pNMHDR, LRESULT *pResult);  // 绘制列表
	afx_msg void OnBnClickedButtonSsdtRefresh();  // 刷新按钮事件
private:
	VOID EnumSSDT();  // 遍历SSDT
	BOOL GetSSDT(IN HANDLE hDevice, OUT PSSDT pSSDT);  // 得到SSDT
	BOOL GetFunAddrByIndex(IN HANDLE hDevice, IN ULONG ulIndex, OUT PULONG pFunAddr);  // 通过索引得到函数地址
	PSYSTEM_MODULE_INFORMATION GetModuleList(IN HMODULE hNtDll);  // 得到系统模块
	CString GetModuleByAddr(IN ULONG ulAddr, IN PSYSTEM_MODULE_INFORMATION pList);  // 通过地址找到所属模块
	VOID GetOrigFunAddr();  // 得到服务函数的原始地址
	DWORD GetKiServiceTable(HMODULE hModule, DWORD dwKSDT);  // 得到KiServiceTable的RVA  
	BOOL GetPEHeaders(HMODULE hModule, PIMAGE_FILE_HEADER& pfh,
		PIMAGE_OPTIONAL_HEADER& poh, PIMAGE_SECTION_HEADER& psh);  // 得到PE文件里的各种头
	BOOL RestoreSSDTItem(IN HANDLE hDevice, IN ULONG ulIndex);  // 重置SSDT项
	
private:
	CBrush m_brush;  // 自定义画刷
	CListCtrl m_ListSSDT;  // SSDT列表框
	CButton m_ButRepairSel;  //修复选中项按钮
};
