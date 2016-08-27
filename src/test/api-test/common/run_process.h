#ifndef run_process_h__
#define run_process_h__

#include <Windows.h>
#include <tchar.h>
#include <string>

BOOL GetCommandOutput(LPCSTR lpszCmd, std::string& result);

void RunCmd(const std::wstring& cmd, LPPROCESS_INFORMATION lpi = NULL, int flags = 0, const std::wstring& working_directory = L"", int nShow = SW_SHOWNORMAL);

DWORD ExecCmd(const std::wstring& cmd, const std::wstring& working_directory = L"", int nShow = SW_SHOWNORMAL);

void DetachCmd(const std::wstring& cmd, LPPROCESS_INFORMATION lpi = NULL);

void ShellCmd(const std::wstring& cmd, int * rc = NULL, LPHANDLE lpProcessHandle = NULL, HWND hWnd = NULL, const std::wstring& working_directory = L"", int nShow = SW_SHOWNORMAL);
#endif // run_process_h__
