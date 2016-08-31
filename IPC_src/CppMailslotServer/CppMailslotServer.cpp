/****************************** Module Header ******************************\
* Module Name:	CppMailslotServer.cpp
* Project:		CppMailslotServer
* Copyright (c) Microsoft Corporation.
* 
* Mailslot is a mechanism for one-way inter-process communication in the 
* local machine or across the computers in the intranet. Any clients can  
* store messages in a mailslot. The creator of the slot, i.e. the server,  
* retrieves the messages that are stored there:
* 
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* This sample demonstrates a mailslot server, \\.\mailslot\HelloWorld. It 
* first creates such a mailslot, then it reads the new messages in the slot  
* every five seconds.
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


BOOL ReadMailslot(HANDLE hMailslot);


int _tmain(int argc, _TCHAR* argv[])
{
	/////////////////////////////////////////////////////////////////////////
	// Create a mailslot.
	// 

	// Prepare the slot name
	CString strSlotName;
	strSlotName.Format(_T("\\\\%s\\mailslot\\%s"), 
		_T("."),			// Server name
		_T("HelloWorld")	// Pipe name
		);

	// Prepare the security attributes

	// If lpSecurityAttributes of CreateMailslot is NULL, the mailslot gets
	// a default security descriptor and the handle cannot be inherited. The 
	// ACLs in the default security descriptor for a mailslot grant full 
	// control to the LocalSystem account, administrators, and the creator 
	// owner. They also grant read access to members of the Everyone group 
	// and the anonymous account. In other words, with NULL as the security 
	// attributes, the mailslot cannot be connected with the WRITE permission
	// across the network, or from a local client running as a lower integiry 
	// level. Here, we fill the security attributes to grant EVERYONE all 
	// access (not just the connect access) to the server. This solves the 
	// cross-network and cross-IL issues, but it creates a security hole 
	// right there: the clients have WRITE_OWNER access and then the server 
	// just lose the control of the mailslot object.
	SECURITY_ATTRIBUTES sa;
	sa.lpSecurityDescriptor = (PSECURITY_DESCRIPTOR)malloc(
		SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(sa.lpSecurityDescriptor, 
		SECURITY_DESCRIPTOR_REVISION);
	// ACL is set as NULL in order to allow all access to the object.
	SetSecurityDescriptorDacl(sa.lpSecurityDescriptor, TRUE, NULL, FALSE);
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;

	// Create the mailslot.
	HANDLE hMailslot = CreateMailslot(
		strSlotName,				// The name of the mailslot
		0,							// No maximum message size 
		MAILSLOT_WAIT_FOREVER,		// No time-out for operations 
		&sa							// Security attributes
		);

	if (hMailslot == INVALID_HANDLE_VALUE) 
	{
		_tprintf(_T("Unable to create mailslot %s w/err 0x%08lx\n"),
			strSlotName, GetLastError());
		return 1;
	}
	_tprintf(_T("The mailslot, %s, is created.\n"), strSlotName);


	/////////////////////////////////////////////////////////////////////////
	// Check messages in the mailslot.
	// 

	// In a loop, check for new messages every 5 seconds.
	while (TRUE)
	{
		_putts(_T("Checking new messages..."));
		ReadMailslot(hMailslot);

		Sleep(5000); // Sleep 5 seconds
	}


	/////////////////////////////////////////////////////////////////////////
	// Close the handle of the mailslot instance.
	// 

	CloseHandle(hMailslot);

	return 0;
}


/*!
 * \brief
 * Read the messages from a mailslot by using the mailslot handle in a call 
 * to the ReadFile function. 
 * 
 * \param hMailslot
 * The handle of the mailslot
 * 
 * \returns
 * If the function succeeds, the return value is nonzero.
 * If the function fails, the return value is zero. To get extended error 
 * information, call GetLastError.
 */
BOOL ReadMailslot(HANDLE hMailslot) 
{
	if (hMailslot == INVALID_HANDLE_VALUE) 
		return FALSE;

	DWORD cbMessageBytes = 0;		// Size of the message in bytes
	DWORD cbBytesRead = 0;			// Number of bytes read from the slot
	DWORD cMessages = 0;			// Number of messages in the mailslot
	DWORD nMessageId = 0;			// Message ID

	BOOL bResult;

	LPTSTR lpszBuffer;				// A buffer used to store one message


	/////////////////////////////////////////////////////////////////////////
	// Check for the number of messages in the mailslot.
	// 

	bResult = GetMailslotInfo(
		hMailslot,					// Handle of the mailslot
		NULL,						// No maximum message size 
		&cbMessageBytes,			// Size of next message 
		&cMessages,					// Number of messages 
		NULL);						// No read time-out 

	if (!bResult) 
	{ 
		_tprintf(_T("GetMailslotInfo failed w/err 0x%08lx\n"), GetLastError()); 
		return FALSE;
	}

	if (cbMessageBytes == MAILSLOT_NO_MESSAGE) 
	{ 
		// There are no new messages in the mailslot at present
		_putts(_T("No new messages."));
		return TRUE; 
	}


	/////////////////////////////////////////////////////////////////////////
	// Retrieve the messages one by one from the mailslot.
	//  
 
	while (cMessages != 0) // If there are still un-read messages in the slot
	{
		nMessageId++;

		// Allocate the memory for the message based on its size info, 
		// cbMessageBytes, which was retrieved from GetMailslotInfo.

		lpszBuffer = (LPTSTR) GlobalAlloc(GPTR, cbMessageBytes);
		if (NULL == lpszBuffer)
			return FALSE;

		bResult = ReadFile(			// Read from the mailslot.
			hMailslot,				// Handle of the slot
			lpszBuffer,				// Buffer to receive data
			cbMessageBytes,			// Size of buffer in bytes
			&cbBytesRead,			// Number of bytes read
			NULL);					// Not overlapped I/O

		if (!bResult) 
		{ 
			_tprintf(_T("ReadFile failed w/err 0x%08lx\n"), GetLastError()); 
			GlobalFree((HGLOBAL)lpszBuffer); 
			return FALSE;
		}

		// Display the message. 
		_tprintf(_T("Message #%ld: %s\n"), nMessageId, lpszBuffer); 

		GlobalFree((HGLOBAL) lpszBuffer); // Free the buffer

		// Get the current number of un-read messages in the slot. The number
		// may not equal the initial message number because new messages may 
		// arrive while we are reading the items in the slot.

		bResult = GetMailslotInfo(
			hMailslot,				// Handle of the mailslot
			NULL,					// No maximum message size 
			&cbMessageBytes,		// Size of next message 
			&cMessages,				// Number of messages 
			NULL);					// No read time-out 

		if (!bResult) 
		{ 
			_tprintf(_T("GetMailslotInfo failed w/err 0x%08lx\n"), GetLastError());
			return FALSE; 
		}
	}

	return TRUE; 
}