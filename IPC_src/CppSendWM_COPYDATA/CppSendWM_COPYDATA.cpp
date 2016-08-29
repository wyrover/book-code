/****************************** Module Header ******************************\
* Module Name:	CppSendWM_COPYDATA.cpp
* Project:		CppSendWM_COPYDATA
* Copyright (c) Microsoft Corporation.
* 
* IPC based on the Windows message WM_COPYDATA is a mechanism for exchanging 
* data among Windows applications in the local machine. The receiving 
* application must be a Windows application. The data being passed must not 
* contain pointers or other references to objects not accessible to the 
* application receiving the data. While WM_COPYDATA is being sent, the 
* referenced data must not be changed by another thread of the sending 
* process. The receiving application should consider the data read-only. The 
* lParam parameter is valid only during the processing of the message. The 
* receiving application should not free the memory referenced by lParam. If 
* the receiving application must access the data after SendMessage returns, 
* it needs to copy the data into a local buffer.
* 
* CppSendWM_COPYDATA demonstrates how a sending process passes the data to  
* other Windows applications by using SendMessage and WM_COPYDATA.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/22/2009 9:04 PM Jialiang Ge Created
\***************************************************************************/

#pragma region Includes
#include "stdafx.h"
#include "CppSendWM_COPYDATA.h"
#include <windowsx.h>
#pragma endregion


// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);

// The struct that carries the data
struct MyStruct
{
	int nNumber;

	TCHAR szMessage[256];
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, DialogProc);
}

//
//   FUNCTION: OnInitDialog(HWND, HWND, LPARAM)
//
//   PURPOSE: Process the WM_INITDIALOG message
//
BOOL OnInitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam)
{
	// Set the small icon for the dialog
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(
		GetModuleHandle(0), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 0, 0,
		LR_DEFAULTCOLOR));

	return TRUE;
}

//
//   FUNCTION: OnCommand(HWND, int, HWND, UINT)
//
//   PURPOSE: Process the WM_COMMAND message
//
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_SENDMSG_BUTTON:

		/////////////////////////////////////////////////////////////////////
		// Find the target Window Handle.
		// 

		HWND hTargetWnd = FindWindow(NULL, _T("CppReceiveWM_COPYDATA"));

		// Validate the Window Handle.
		if (hTargetWnd == NULL)
		{
			MessageBox(NULL, _T("Unable to find the target Window!"),
				_T("Warning"), MB_OK);
			break;
		}


		/////////////////////////////////////////////////////////////////////
		// Prepare the COPYDATASTRUCT struct with the data to be sent.
		// 

		MyStruct myStruct;

		// Get the value of nNumber
		BOOL bTranslated;
		myStruct.nNumber = GetDlgItemInt(hWnd, IDC_NUMBER_EDIT,
			&bTranslated, TRUE);
		if (!bTranslated)
		{
			MessageBox(NULL, _T("Invalid value of nNumber!"), 
				_T("Warning"), MB_OK);
			break;
		}

		// Get the text of strMessage
		GetDlgItemText(hWnd, IDC_MESSAGE_EDIT, myStruct.szMessage, 256);

		COPYDATASTRUCT cds;
		cds.cbData = sizeof(myStruct);
		cds.lpData = (LPVOID)&myStruct;


		/////////////////////////////////////////////////////////////////////
		// Send the COPYDATASTRUCT struct through the WM_COPYDATA message to
		// the receiving Window.
		// 

		SendMessage(hTargetWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);

		DWORD result = GetLastError();
		if (result != 0)
		{
			MessageBox(NULL, _T("SendMessage failed"), _T("Warning"), MB_OK);
		}

		break;
	}
}

//
//   FUNCTION: OnClose(HWND)
//
//   PURPOSE: Process the WM_CLOSE message
//
void OnClose(HWND hWnd)
{
	EndDialog(hWnd, 0);
}

//
//  FUNCTION: DialogProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main dialog.
//
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Handle the WM_INITDIALOG message in OnInitDialog
		HANDLE_MSG (hWnd, WM_INITDIALOG, OnInitDialog);

		// Handle the WM_COMMAND message in OnCommand
		HANDLE_MSG (hWnd, WM_COMMAND, OnCommand);

		// Handle the WM_CLOSE message in OnClose
		HANDLE_MSG (hWnd, WM_CLOSE, OnClose);

	default:
		return FALSE;
	}
	return 0;
}