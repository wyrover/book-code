//handler.cpp
#include "std.h"


//암시적  
void OnImplicit(HWND hDlg)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO			si = { sizeof(si) };
	BOOL b = CreateProcess(0, "A.exe", 0, 0, 
				FALSE, NORMAL_PRIORITY_CLASS, 0, 0, 
				&si, &pi);
	if( b )
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

//명시적
void OnExplicit(HWND hDlg)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO			si = { sizeof(si) };
	BOOL b = CreateProcess(0, "B.exe", 0, 0, 
				FALSE, NORMAL_PRIORITY_CLASS, 0, 0, 
				&si, &pi);
	if( b )
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

int OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

int OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
	case IDC_BUTTON1: OnImplicit(hDlg);			break;
	case IDC_BUTTON2: OnExplicit(hDlg);			break;
		
	case IDCANCEL: EndDialog(hDlg, IDCANCEL);	return 0;
	}
	return true;
}