// sysinfo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Windows.h>
#include <WinBase.h>
#include <winternl.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#define STATUS_SUCCESS               ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH  ((NTSTATUS)0xC0000004L)
#define N_ELEMENTS(x)  sizeof(x)/sizeof(x[0])


typedef struct _SYSTEM_FILECACHE_INFORMATION {
	SIZE_T CurrentSize;
	SIZE_T PeakSize;
	ULONG PageFaultCount;
	SIZE_T MinimumWorkingSet;
	SIZE_T MaximumWorkingSet;
	SIZE_T CurrentSizeIncludingTransitionInPages;
	SIZE_T PeakSizeIncludingTransitionInPages;
	ULONG TransitionRePurposeCount;
	ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION, *PSYSTEM_FILECACHE_INFORMATION;

typedef struct _SYSTEM_PAGEFILE_INFORMATION {
	ULONG                   NextEntryOffset;
	ULONG                   TotalSize;
	ULONG                   TotalInUse;
	ULONG                   PeakUsage;
	UNICODE_STRING          PageFileName;
} SYSTEM_PAGEFILE_INFORMATION, *PSYSTEM_PAGEFILE_INFORMATION;

typedef long(__stdcall *NtQuerySystemInformationTT)(DWORD, PVOID, DWORD, DWORD*);

NtQuerySystemInformationTT getNtQuerySysInfoFn()
{
	HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
	NtQuerySystemInformationTT NtQuerySystemInformation = (NtQuerySystemInformationTT)GetProcAddress(hDll, "NtQuerySystemInformation");
	return NtQuerySystemInformation;
}

struct _LOCAL_SYSTEM_PERFORMANCE_INFO {
	/*0x00 		*/	LARGE_INTEGER IdleProcessTime;
	/*0x08 		*/	LARGE_INTEGER IoReadTransferCount;
	/*0x10		*/	LARGE_INTEGER IoWriteTransferCount;
	/*0x18		*/	LARGE_INTEGER IoOtherTransferCount;
	/*0x20		*/	ULONG IoReadOperationCount;
	/*0x24		*/	ULONG IoWriteOperationCount;
	/*0x28		*/	ULONG IoOtherOperationCount;
	/*0x2C		*/	ULONG AvailablePages;
	/*0x30		*/	ULONG CommittedPages;
	/*0x34		*/	ULONG CommitLimit;
	/*0x38		*/	ULONG PeakCommitment;
	/*0x3C		*/	ULONG PageFaultCount;
	/*0x40		*/	ULONG CopyOnWriteCount;
	/*0x44		*/	ULONG TransitionCount;
	/*0x48		*/	ULONG CacheTransitionCount;
	/*0x4C		*/	ULONG DemandZeroCount;
	/*0x50		*/	ULONG PageReadCount;
	/*0x54		*/	ULONG PageReadIoCount;
	/*0x58		*/	ULONG CacheReadCount;
	/*0x5C		*/	ULONG CacheIoCount;
	/*0x60		*/	ULONG DirtyPagesWriteCount;
	/*0x64		*/	ULONG DirtyWriteIoCount;
	/*0x68		*/	ULONG MappedPagesWriteCount;
	/*0x6C		*/	ULONG MappedWriteIoCount;
	/*0x70		*/	ULONG PagedPoolPages;
	/*0x74		*/	ULONG NonPagedPoolPages;
	/*0x78		*/	ULONG PagedPoolAllocs;
	/*0x7C		*/	ULONG PagedPoolFrees;
	/*0x80		*/	ULONG NonPagedPoolAllocs;
	/*0x84		*/	ULONG NonPagedPoolFrees;
	/*0x88		*/	ULONG FreeSystemPtes;
	/*0x8C		*/	ULONG ResidentSystemCodePage;
	/*0x90		*/	ULONG TotalSystemDriverPages;
	/*0x94		*/	ULONG TotalSystemCodePages;
	/*0x98		*/	ULONG NonPagedPoolLookasideHits;
	/*0x9C		*/	ULONG PagedPoolLookasideHits;
	/*0xA0		*/	ULONG AvailablePagedPoolPages;
	/*0xA4		*/	ULONG ResidentSystemCachePage;
	/*0xA8		*/	ULONG ResidentPagedPoolPage;
	/*0xAC		*/	ULONG ResidentSystemDriverPage;
	/*0xB0		*/	ULONG CcFastReadNoWait;
	/*0xB4		*/	ULONG CcFastReadWait;
	/*0xB8		*/	ULONG CcFastReadResourceMiss;
	/*0xBC		*/	ULONG CcFastReadNotPossible;
	/*0xC0		*/	ULONG CcFastMdlReadNoWait;
	/*0xC4		*/	ULONG CcFastMdlReadWait;
	/*0xC8		*/	ULONG CcFastMdlReadResourceMiss;
	/*0xCC		*/	ULONG CcFastMdlReadNotPossible;
	/*0xD0		*/	ULONG CcMapDataNoWait;
	/*0xD4		*/	ULONG CcMapDataWait;
	/*0xD8		*/	ULONG CcMapDataNoWaitMiss;
	/*0xDC		*/	ULONG CcMapDataWaitMiss;
	/*0xE0		*/	ULONG CcPinMappedDataCount;
	/*0xE4		*/	ULONG CcPinReadNoWait;
	/*0xE8		*/	ULONG CcPinReadWait;
	/*0xEC		*/	ULONG CcPinReadNoWaitMiss;
	/*0xF0		*/	ULONG CcPinReadWaitMiss;
	/*0xF4		*/	ULONG CcCopyReadNoWait;
	/*0xF8		*/	ULONG CcCopyReadWait;
	/*0xFC		*/	ULONG CcCopyReadNoWaitMiss;
	/*0x0100	*/	ULONG CcCopyReadWaitMiss;
	/*0x0104	*/	ULONG CcMdlReadNoWait;
	/*0x0108	*/	ULONG CcMdlReadWait;
	/*0x010C	*/	ULONG CcMdlReadNoWaitMiss;
	/*0x0110	*/	ULONG CcMdlReadWaitMiss;
	/*0x0114	*/	ULONG CcReadAheadIos;
	/*0x0118	*/	ULONG CcLazyWriteIos;
	/*0x011C	*/	ULONG CcLazyWritePages;
	/*0x0120	*/	ULONG CcDataFlushes;
	/*0x0124	*/	ULONG CcDataPages;
	/*0x0128	*/	ULONG ContextSwitches;
	/*0x012C	*/	ULONG FirstLevelTbFills;
	/*0x0130	*/	ULONG SecondLevelTbFills;
	/*0x0134	*/	ULONG SystemCalls;
	/*0x0138	*/	ULONGLONG CcTotalDirtyPages;
	/*0x0140	*/	ULONGLONG CcDirtyPageThreshold;
	/*0x0148	*/	LONGLONG ResidentAvailablePages;
	/*0x0150	*/	ULONGLONG SharedCommittedPages;
};

struct _LOCAL_SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX
{
	LARGE_INTEGER IdleTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER DpcTime;
	LARGE_INTEGER InterruptTime;
	ULONG InterruptCount;
	ULONG Spare0;
	LARGE_INTEGER AvailableTime;
	LARGE_INTEGER Spare1;
	LARGE_INTEGER Spare2;
};

typedef struct _QUERY_PERFORMANCE_COUNTER_FLAGS {
	union {
		struct {
			ULONG KernelTransition : 1;
			ULONG Reserved : 31;
		};
		ULONG ul;
	};
} QUERY_PERFORMANCE_COUNTER_FLAGS;

struct _LOCAL_SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION
{
	ULONG Version;								//	input, must be 1
	QUERY_PERFORMANCE_COUNTER_FLAGS Flags;		//	output
	QUERY_PERFORMANCE_COUNTER_FLAGS ValidFlags;	//	output
};

typedef LONG KPRIORITY; // Thread priority
typedef struct _SYSTEM_PROCESS_INFORMATION_DETAILED {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
	ULONG HandleCount;
	BYTE Reserved4[4];
	PVOID Reserved5[11];
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER Reserved6[6];
} SYSTEM_PROCESS_INFORMATION_DETAILED, *PSYSTEM_PROCESS_INFORMATION_DETAILED;


static  int main1()
{
	HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
	NtQuerySystemInformationTT NtQuerySystemInformation = (NtQuerySystemInformationTT)GetProcAddress(hDll, "NtQuerySystemInformation");

	size_t min = 0;
	size_t max = 0;
	DWORD flags = 0;
	GetSystemFileCacheSize((PSIZE_T)&min, (PSIZE_T)&max, &flags);
	std::cout << "SystemFileCacheSize - min:" << min << ", max:" << max << ", flags" << flags << std::endl;

	ULONGLONG totalMem = 0;
	GetPhysicallyInstalledSystemMemory(&totalMem);
	std::cout << "GetPhysicallyInstalledSystemMemory- " << totalMem / 1024 / 1024 << " GB" << std::endl;

	static const int SystemPageFileInformationEx = 0x90;
	SYSTEM_PAGEFILE_INFORMATION pageFileInfo;
	ULONG returnedLen = 0;
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemPageFileInformationEx, &pageFileInfo, sizeof(SYSTEM_PAGEFILE_INFORMATION), &returnedLen);

	std::wcout << "PageFileInfo:"
		<< "\n\tTotalSize: " << pageFileInfo.TotalSize
		<< "\n\tTotalInUse: " << pageFileInfo.TotalInUse
		<< "\n\tPeakUsage: " << pageFileInfo.PeakUsage << std::endl;


	static const int SystemPerformanceInformation = 0x02;
	_LOCAL_SYSTEM_PERFORMANCE_INFO  perfInfo1;
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemPerformanceInformation, &perfInfo1, sizeof(_LOCAL_SYSTEM_PERFORMANCE_INFO), &returnedLen);

	std::cout << "System Information:" << std::endl
		<< "\tIdleTime:" << perfInfo1.IdleProcessTime.QuadPart << std::endl
		<< "\tAvailablePages:" << perfInfo1.AvailablePages << std::endl
		<< "\tContextSwitches:" << perfInfo1.ContextSwitches << std::endl
		<< "\tSystemCalls:" << perfInfo1.SystemCalls << std::endl;

	::Sleep(1000);

	_LOCAL_SYSTEM_PERFORMANCE_INFO  perfInfo2;
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemPerformanceInformation, &perfInfo2, sizeof(_LOCAL_SYSTEM_PERFORMANCE_INFO), &returnedLen);

	std::cout << "System Information (Delta):" << std::endl
		<< "\tIdleTime:" << (perfInfo2.IdleProcessTime.QuadPart - perfInfo1.IdleProcessTime.QuadPart) / 10000 << std::endl
		<< "\tAvailablePages:" << (perfInfo2.AvailablePages - perfInfo1.AvailablePages) << std::endl
		<< "\tContextSwitches:" << (perfInfo2.ContextSwitches - perfInfo1.ContextSwitches) << std::endl
		<< "\tSystemCalls:" << (perfInfo2.SystemCalls - perfInfo1.SystemCalls) << std::endl;


	static const int SystemProcessorPerformanceInformationEx = 0x8D;

	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION procPerfInfo;
	NtQuerySystemInformation(SystemProcessorPerformanceInformation, &procPerfInfo, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION), &returnedLen);

	std::cout << "Processor Information: " << std::endl
		<< "\tIdleTimeTime" << procPerfInfo.IdleTime.QuadPart << std::endl
		<< "\tKernelTime" << procPerfInfo.KernelTime.QuadPart << std::endl
		<< "\tUserTime" << procPerfInfo.UserTime.QuadPart << std::endl;

	::Sleep(1000);

	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION procPerfInfo2;
	NtQuerySystemInformation(SystemProcessorPerformanceInformation, &procPerfInfo2, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION), &returnedLen);

	auto idleTime = (procPerfInfo2.IdleTime.QuadPart - procPerfInfo.IdleTime.QuadPart);
	auto kernelTime = (procPerfInfo2.KernelTime.QuadPart - procPerfInfo.KernelTime.QuadPart) - idleTime;
	auto userTime = (procPerfInfo2.UserTime.QuadPart - procPerfInfo.UserTime.QuadPart);
	std::cout << "Processor Information: " << std::endl
		<< "\tIdleTimeTime - " << idleTime / 10000 << std::endl
		<< "\tKernelTime   - " << kernelTime / 10000 << std::endl
		<< "\tUserTime     - " << userTime / 10000 << std::endl;

	return 0;
}


class ProcessInfo
{
	std::wstring _name;
	int _id;
	int _numThreads;
	int _numHandles;
	LONGLONG _userTime;
	LONGLONG _kernelTime;

public:
	ProcessInfo(PSYSTEM_PROCESS_INFORMATION_DETAILED pdi)
	{
		_name.assign(((pdi->ImageName.MaximumLength > 0 && pdi->ImageName.Length > 0 && pdi->ImageName.Buffer) ? pdi->ImageName.Buffer : L""));
		_id = (int)pdi->UniqueProcessId;
		_numThreads = pdi->NumberOfThreads;
		_numHandles = pdi->HandleCount;
		_userTime = pdi->UserTime.QuadPart / 10000;
		_kernelTime = pdi->KernelTime.QuadPart / 10000;
	}

	std::wstring name() const
	{
		return _name;
	}

	int id() const
	{
		return _id;
	}

	int numHandles() const
	{
		return _numHandles;
	}

	int numThreads() const
	{
		return _numThreads;
	}

	LONGLONG userTime() const
	{
		return _userTime;
	}

	LONGLONG kernelTime() const
	{
		return _kernelTime;
	}
};



static int main_process_info()
{
	static const int SystemProcessInformation = 0x5;
	SYSTEM_PROCESS_INFORMATION_DETAILED pspidArr[1024] = {};
	ULONG returnedLen = 0;

	auto NtQuerySystemInformation = getNtQuerySysInfoFn();
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessInformation, (PVOID)pspidArr, sizeof(pspidArr), &returnedLen);

	int numProcs = returnedLen / sizeof(SYSTEM_PROCESS_INFORMATION_DETAILED);
	std::vector<ProcessInfo> processInfoVec;
	processInfoVec.reserve(numProcs);
	PSYSTEM_PROCESS_INFORMATION_DETAILED pspid = pspidArr;
	for (;; pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILED)(pspid->NextEntryOffset + (PBYTE)pspid))
	{
		if ((pspid->ImageName.MaximumLength > 0 && pspid->ImageName.Length > 0 && pspid->ImageName.Buffer != nullptr))
			processInfoVec.emplace_back(pspid);

		if (pspid->NextEntryOffset == 0) break;
	}

	auto ascendingSort = [](const ProcessInfo& one, const ProcessInfo& two)
	{
		if (one.name().empty())
			return true;
		else if (two.name().empty())
			return false;

		return _wcsicmp(one.name().c_str(), two.name().c_str()) < 0;
	};

	auto descedingSort = [](const ProcessInfo& one, const ProcessInfo& two)
	{
		if (one.name().empty())
			return false;
		else if (two.name().empty())
			return true;

		return _wcsicmp(one.name().c_str(), two.name().c_str()) > 0;
	};

	std::sort(processInfoVec.begin(), processInfoVec.end(), descedingSort);

#define FORMAT_MACRO(width) std::left << std::setw(width) << std::setfill(L' ')
	std::wcout
		<< FORMAT_MACRO(06) << "pid"
		<< FORMAT_MACRO(40) << "name"
		<< FORMAT_MACRO(06) << "#thds"
		<< FORMAT_MACRO(10) << "userTime"
		<< FORMAT_MACRO(10) << "krnlTime" << std::endl;
	for (int idx = 0; idx < min(50, processInfoVec.size()); ++idx)
	{
		const auto& info = processInfoVec[idx];
		std::wcout
			<< FORMAT_MACRO(06) << info.id()
			<< FORMAT_MACRO(06) << info.numThreads()
			<< FORMAT_MACRO(10) << info.userTime()
			<< FORMAT_MACRO(10) << info.kernelTime()
			<< FORMAT_MACRO(40) << info.name()
			<< std::endl;
	}

	return 0;

}


static int main_page_file_info()
{
	auto querySysInfo = getNtQuerySysInfoFn();

	static const int SystemPageFileInformation = 0x12;
	SYSTEM_PAGEFILE_INFORMATION pageFileInfo;
	ULONG returnedLen = 0;
	querySysInfo(SystemPageFileInformation, &pageFileInfo, sizeof(SYSTEM_PAGEFILE_INFORMATION), &returnedLen);

	std::wcout << "PageFile Stats: " << std::endl;
	std::wcout << "\tTotal: " << pageFileInfo.TotalSize << std::endl;
	std::wcout << "\tInUse: " << pageFileInfo.TotalInUse << std::endl;
	std::wcout << "\tPeakUsage: " << pageFileInfo.PeakUsage << std::endl;

	return 0;
}




static void mainProcessorInfo()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;

	std::vector<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> procPerfInfoVec;
	procPerfInfoVec.resize(numCPU);

	auto querySysInfo = getNtQuerySysInfoFn();
	static const int SystemProcessorPerformanceInformation = 0x08;

	ULONG returnedLen = 0;
	querySysInfo(SystemProcessorPerformanceInformation, &procPerfInfoVec[0], sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * numCPU, &returnedLen);

	Sleep(1000);

	for (int idx = 0; idx < 20; ++idx)
	{
		std::vector<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> procPerfInfoVecNew;
		procPerfInfoVecNew.resize(numCPU);
		querySysInfo(SystemProcessorPerformanceInformation, &procPerfInfoVecNew[0], sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * numCPU, &returnedLen);

		for (int pdx = 0; pdx < numCPU; ++pdx)
		{
			auto idleTime = (procPerfInfoVecNew[pdx].IdleTime.QuadPart - procPerfInfoVec[pdx].IdleTime.QuadPart);
			auto userTime = (procPerfInfoVecNew[pdx].UserTime.QuadPart - procPerfInfoVec[pdx].UserTime.QuadPart);
			auto krnlTime = (procPerfInfoVecNew[pdx].KernelTime.QuadPart - procPerfInfoVec[pdx].KernelTime.QuadPart);
			std::cout << "Proc[" << pdx << "]"
				<< "  UserTime- " << userTime / 10000
				<< "  KrnlTime- " << (krnlTime - idleTime) / 10000
				<< "  IdleTime- " << idleTime / 1000
				<< std::endl;
		}

		std::swap(procPerfInfoVecNew, procPerfInfoVec);

		Sleep(1000);
	}
}

static void ClearConsole()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the
									 cursor */
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in
									 the current buffer */

									 /* get the number of character cells in the current buffer */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */

	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten);

	/* get the current text attribute */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);

	/* now set the buffer's attributes accordingly */

	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten);

	/* put the cursor at (0, 0) */

	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}

int main_console()
{
	for (int idx = 0; idx < 20; ++idx)
	{
		ClearConsole();
		main_process_info();
		Sleep(1000);
	}

	return 0;
}





int main()
{

	//	get command line of exe
	//	http://stackoverflow.com/a/13408150/916549
	//
	static const int SystemProcessInformation = 0x5;
	SYSTEM_PROCESS_INFORMATION_DETAILED pspidArr[1024] = {};
	ULONG returnedLen = 0;


	HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
	typedef NTSTATUS(*NtQueryInformationProcess_t)(
		_In_      HANDLE           ProcessHandle,
		_In_      PROCESSINFOCLASS ProcessInformationClass,
		_Out_     PVOID            ProcessInformation,
		_In_      ULONG            ProcessInformationLength,
		_Out_opt_ PULONG           ReturnLength
		);

	NtQueryInformationProcess_t NtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(hDll, "NtQueryInformationProcess");

	auto NtQuerySystemInformation = getNtQuerySysInfoFn();
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessInformation, (PVOID)pspidArr, sizeof(pspidArr), &returnedLen);

	int numProcs = returnedLen / sizeof(SYSTEM_PROCESS_INFORMATION_DETAILED);
	std::vector<ProcessInfo> processInfoVec;
	processInfoVec.reserve(numProcs);
	PSYSTEM_PROCESS_INFORMATION_DETAILED pspid = pspidArr;
	for (;; pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILED)(pspid->NextEntryOffset + (PBYTE)pspid))
	{
		if ((pspid->ImageName.MaximumLength > 0 && pspid->ImageName.Length > 0 && pspid->ImageName.Buffer != nullptr))
			processInfoVec.emplace_back(pspid);

		if (pspid->NextEntryOffset == 0) break;
	}


	PEB copiedPEB;
	RTL_USER_PROCESS_PARAMETERS copiedProcParams;
	for (const auto& proc : processInfoVec)
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc.id());
		if (hProcess)
		{
			PROCESS_BASIC_INFORMATION info;
			LONG status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &info, sizeof(PROCESS_BASIC_INFORMATION), nullptr);
			PPEB ppeb = info.PebBaseAddress;
			PPEB ppebCopy = &copiedPEB;

			BOOL result = ReadProcessMemory(hProcess, ppeb, ppebCopy, sizeof(PEB), NULL);

			PRTL_USER_PROCESS_PARAMETERS pRtlProcParam = ppebCopy->ProcessParameters;
			PRTL_USER_PROCESS_PARAMETERS pRtlProcParamCopy = &copiedProcParams;

			result = ReadProcessMemory(hProcess, pRtlProcParam, pRtlProcParamCopy, sizeof(RTL_USER_PROCESS_PARAMETERS), NULL);

			PWSTR wBuffer = pRtlProcParamCopy->CommandLine.Buffer;
			USHORT len = pRtlProcParamCopy->CommandLine.Length;
			PWSTR wBufferCopy = (PWSTR)malloc(len);
			result = ReadProcessMemory(hProcess, wBuffer, wBufferCopy, // command line goes here
									   len, NULL);

			std::wstring cmdLineStr(wBufferCopy, wBufferCopy + len / 2);
			free(wBufferCopy);
			wBufferCopy = nullptr;

			_tprintf(L"%ls --> cmd line: %ls\n", proc.name().c_str(), cmdLineStr.c_str());
		}
		else
		{
			_tprintf(L"%ls\n", proc.name().c_str());
		}
		CloseHandle(hProcess);
	}
}