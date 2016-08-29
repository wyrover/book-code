/****************************** Module Header ******************************\
* Module Name:	CppMailslotClient.cpp
* Project:		CppMailslotClient
* Copyright (c) Microsoft Corporation.
* 
* Mailslot is a mechanism for one-way inter-process communication in the 
* local machine or across the computers in the intranet. Any clients can  
* store messages in a mailslot. The creator of the slot, i.e. the server,  
* retrieves the messages that are stored there:
* 
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* This sample demonstrates a mailslot client that connects and writes to the 
* mailslot \\.\mailslot\HelloWorld. 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 1/21/2009 11:04 PM Jialiang Ge Created
\***************************************************************************/

#pragma region Includes
#include "stdafx.h"

#include <windows.h>
#include <atlstr.h>
#include <strsafe.h>
#pragma endregion


BOOL WriteMailslot(HANDLE hMailslot, LPTSTR lpszMessage);


int _tmain(int argc, _TCHAR* argv[])
{
	/////////////////////////////////////////////////////////////////////////
	// Open the mailslot.
	// 

	// Prepare the slot name
	CString strMailslotName;
	strMailslotName.Format(_T("\\\\%s\\mailslot\\%s"), 
		_T("."),			// Server name
		_T("HelloWorld")	// Pipe name
		);

	HANDLE hFile = CreateFile(
		strMailslotName,			// The name of the mailslot
		GENERIC_WRITE,				// Write access (Mailslot is a mechanism 
									// for one-way IPC. The client is just
									// responsible for writing to the slot)
		FILE_SHARE_READ,			// Share mode
		NULL,						// Default security attributes
		OPEN_EXISTING,				// Opens existing mailslot 
		FILE_ATTRIBUTE_NORMAL,		// The file has no other attributes set
		NULL);						// No template file

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		_tprintf(_T("Unable to open mailslot %s w/err 0x%08lx\n"),
			strMailslotName, GetLastError());
		return 1;
	}


	/////////////////////////////////////////////////////////////////////////
	// Write messages to the mailslot.
	// 

	WriteMailslot(hFile, _T("Message 1 for mailslot"));
	WriteMailslot(hFile, _T("Message 2 for mailslot"));

	Sleep(3000); // Sleep 3 seconds

	WriteMailslot(hFile, _T("Message 3 for mailslot"));


	/////////////////////////////////////////////////////////////////////////
	// Close the slot.
	// 

	CloseHandle(hFile); 

	return 0;
}


/*!
 * \brief
 * Write a message to the specified mailslot.
 * 
 * \param hMailslot
 * The handle of the mailslot
 * 
 * \param lpszMessage
 * The message to be written to the slot
 * 
 * \returns
 * If the function succeeds, the return value is nonzero.
 * If the function fails, the return value is zero. To get extended error 
 * information, call GetLastError.
 */
BOOL WriteMailslot(HANDLE hMailslot, LPTSTR lpszMessage)
{
	DWORD cbMessageBytes = 0;		// Size of the message in bytes
	DWORD cbBytesWritten = 0;		// Number of bytes written to the slot

	BOOL bResult; 

	// Calculate the size of the message in bytes
	cbMessageBytes = (_tcslen(lpszMessage) + 1 /* NULL */) * sizeof(TCHAR);

	bResult = WriteFile(			// Write to the mailslot.
		hMailslot,					// Handle of the slot
		lpszMessage,				// Message to be written
		cbMessageBytes,				// Number of bytes to write
		&cbBytesWritten,			// Number of bytes written
		NULL); 						// Not overlapped 

	if (!bResult/*Failed*/ || cbMessageBytes != cbBytesWritten/*Failed*/) 
	{ 
		_tprintf(_T("WriteFile failed w/err 0x%08lx\n"), GetLastError());
		return FALSE; 
	}

	_tprintf(_T("The message, %s, is written to the slot.\n"), 
		lpszMessage); 

	return TRUE;
}