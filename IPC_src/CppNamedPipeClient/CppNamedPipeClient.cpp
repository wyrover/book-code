/****************************** Module Header ******************************\
* Module Name:	CppNamedPipeClient.cpp
* Project:		CppNamedPipeClient
* Copyright (c) Microsoft Corporation.
* 
* Named pipe is a mechanism for one-way or bi-directional inter-process 
* communication between the pipe server and one or more pipe clients in the
* local machine or across the computers in the intranet:
* 
* PIPE_ACCESS_INBOUND:
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* PIPE_ACCESS_OUTBOUND:
* Client (GENERIC_READ) <--- Server (GENERIC_WRITE)
* 
* PIPE_ACCESS_DUPLEX:
* Client (GENERIC_READ or GENERIC_WRITE, or both) <--> 
* Server (GENERIC_READ and GENERIC_WRITE)
* 
* This sample demonstrates a named pipe client that attempts to connect to   
* the pipe server, \\.\pipe\HelloWorld, with the GENERIC_READ and 
* GENERIC_WRITE permissions. The client writes a message to the pipe server 
* and receive its response.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 1/11/2009 11:20 PM Jialiang Ge Created
\***************************************************************************/

#pragma region Includes
#include "stdafx.h"

#include <windows.h>
#include <atlstr.h>
#include <strsafe.h>
#pragma endregion


#define BUFFER_SIZE		1024 // 1K

int _tmain(int argc, _TCHAR* argv[])
{
	/////////////////////////////////////////////////////////////////////////
	// Try to open a named pipe.
	// 

	// Prepare the pipe name
	CString strPipeName;
	strPipeName.Format(_T("\\\\%s\\pipe\\%s"), 
		_T("."),			// Server name
		_T("HelloWorld")	// Pipe name
		);
	
	HANDLE hPipe; 
	while (TRUE) 
	{
		hPipe = CreateFile( 
			strPipeName,			// Pipe name 
			GENERIC_READ |			// Read and write access 
			GENERIC_WRITE,
			0,						// No sharing 
			NULL,					// Default security attributes
			OPEN_EXISTING,			// Opens existing pipe 
			0,						// Default attributes 
			NULL);					// No template file 

		// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE) 
			break; 
 
		if (// Exit if an error other than ERROR_PIPE_BUSY occurs
			GetLastError() != ERROR_PIPE_BUSY 
			||
			// All pipe instances are busy, so wait for 5 seconds
			!WaitNamedPipe(strPipeName, 5000)) 
		{
			_tprintf(_T("Unable to open named pipe %s w/err 0x%08lx\n"),
				strPipeName, GetLastError());
			return 1;
		}
	}
	_tprintf(_T("The named pipe, %s, is connected.\n"), strPipeName);
	

	/////////////////////////////////////////////////////////////////////////
	// Sets the read mode and the blocking mode of the specified named pipe.
	// 

	// Set data to be read from the pipe as a stream of messages
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL bResult = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
	if (!bResult) 
	{
		_tprintf(_T("SetNamedPipeHandleState failed w/err 0x%08lx\n"), 
			GetLastError()); 
		return 1;
	}


	/////////////////////////////////////////////////////////////////////////
	// Send a message to the pipe server and receive its response.
	// 

	// A char buffer of BUFFER_SIZE chars, aka BUFFER_SIZE * sizeof(TCHAR) 
	// bytes. The buffer should be big enough for ONE request to the server.

	TCHAR chRequest[BUFFER_SIZE];	// Client -> Server
	DWORD cbBytesWritten, cbRequestBytes;
	TCHAR chReply[BUFFER_SIZE];		// Server -> Client
	DWORD cbBytesRead, cbReplyBytes;

	// Send one message to the pipe.

	StringCchCopy(chRequest, BUFFER_SIZE, _T("Default request from client"));
	cbRequestBytes = sizeof(TCHAR) * (lstrlen(chRequest) + 1);

	bResult = WriteFile(			// Write to the pipe.
		hPipe,						// Handle of the pipe
		chRequest,					// Message to be written
		cbRequestBytes,				// Number of bytes to write
		&cbBytesWritten,			// Number of bytes written
		NULL);						// Not overlapped 

	if (!bResult/*Failed*/ || cbRequestBytes != cbBytesWritten/*Failed*/) 
	{
		_tprintf(_T("WriteFile failed w/err 0x%08lx\n"), GetLastError());
		return 1;
	}

	_tprintf(_T("Sends %ld bytes; Message: \"%s\"\n"), 
		cbBytesWritten, chRequest);

	// Receive the response from the server.

	cbReplyBytes = sizeof(TCHAR) * BUFFER_SIZE;
	do
	{
		bResult = ReadFile(			// Read from the pipe.
			hPipe,					// Handle of the pipe
			chReply,				// Buffer to receive the reply
			cbReplyBytes,			// Size of buffer 
			&cbBytesRead,			// Number of bytes read 
			NULL);					// Not overlapped 

		if (!bResult && GetLastError() != ERROR_MORE_DATA) 
		{
			_tprintf(_T("ReadFile failed w/err 0x%08lx\n"), GetLastError());
			break;
		}

		_tprintf(_T("Receives %ld bytes; Message: \"%s\"\n"), 
			cbBytesRead, chReply);

	} while (!bResult);  // Repeat loop if ERROR_MORE_DATA 


	/////////////////////////////////////////////////////////////////////////
	// Close the pipe.
	// 

	CloseHandle(hPipe); 

	return 0;
}

