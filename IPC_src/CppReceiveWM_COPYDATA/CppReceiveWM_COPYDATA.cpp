/****************************** Module Header ******************************\
* Module Name:	CppReceiveWM_COPYDATA.cpp
* Project:		CppReceiveWM_COPYDATA
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
* CppReceiveWM_COPYDATA demonstrates how a receiving process handles the 
* WM_COPYDATA message to get the passed data.
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
#include "CppReceiveWM_COPYDATA.h"
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
//   FUNCTION: OnCopyData(HWND, HWND, PCOPYDATASTRUCT)
//
//   PURPOSE: Process the WM_COPYDATA message
//
//   COMMENTS:
//
//        hwndFrom - Handle to the window passing the data. 
//        pcds - Pointer to a COPYDATASTRUCT structure that contains the data 
//               to be passed. 
//
BOOL OnCopyData(HWND hWnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
	// If the size matches
	if (pcds->cbData == sizeof(MyStruct))
	{
		// Get the MyStruct value from pcds->lpData
		MyStruct myStruct;
		memcpy(&myStruct, pcds->lpData, sizeof(MyStruct));

		// Display the MyStruct value in the form
		SetDlgItemInt(hWnd, IDC_NUMBER_STATIC, myStruct.nNumber, TRUE);
		SetDlgItemText(hWnd, IDC_MESSAGE_STATIC, myStruct.szMessage);
	}

	return TRUE;
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

		// Handle the WM_COPYDATA message in OnCopyData
		HANDLE_MSG (hWnd, WM_COPYDATA, OnCopyData);

		// Handle the WM_CLOSE message in OnClose
		HANDLE_MSG (hWnd, WM_CLOSE, OnClose);

	default:
		return FALSE;
	}
	return 0;
}