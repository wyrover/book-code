#include "InternalMonitor.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <Windows.h>

InternalMonitor::InternalMonitor()
{
}


InternalMonitor::~InternalMonitor()
{
}

const unsigned int InternalMonitor::getBrightness()
{

	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL; 
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	SECURITY_ATTRIBUTES saAttr{ sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
	{
		std::cerr << "Could not CreatePipe" << std::endl;
	}
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
	{
		std::cerr << "Could not SetHandleInformation" << std::endl;
	}

	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
	{
		std::cerr << "Could not CreatePipe" << std::endl;
	}
	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
	{
		std::cerr << "Could not SetHandleInformation" << std::endl;
	}

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = g_hChildStd_OUT_Wr;
	si.hStdOutput = g_hChildStd_OUT_Wr;
	si.hStdInput = g_hChildStd_IN_Rd;
	si.dwFlags |= STARTF_USESTDHANDLES | SW_HIDE | STARTF_USESHOWWINDOW;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	wchar_t cmd[] = L"powershell \"(Get-WmiObject -ns root/wmi -class wmiMonitorBrightness).CurrentBrightness\"";

	BOOL result = CreateProcess(NULL,
		cmd,
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		CREATE_NO_WINDOW,// creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&si,           // STARTUPINFO pointer 
		&pi);          // receives PROCESS_INFORMATION

	if (!result)
	{
		std::cerr << "Could not CreateProcess" << std::endl;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	CloseHandle(g_hChildStd_IN_Wr);


	DWORD dwRead;
	CHAR chBuf[256];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, 256, &dwRead, NULL);
	
	return std::stoi( std::string(chBuf) );
}
