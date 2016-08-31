/****************************** Module Header ******************************\
* Module Name:	CppFileMappingClient.cpp
* Project:		CppFileMappingClient
* Copyright (c) Microsoft Corporation.
* 
* File mapping is a mechanism for one-way or bi-directional inter-process 
* communication among two or more processes in the local machine. To share a 
* file or memory, all of the processes must use the name or the handle of the 
* same file mapping object.
* 
* To share a file, the first process creates or opens a file by using the 
* CreateFile function. Next, it creates a file mapping object by using the 
* CreateFileMapping function, specifying the file handle and a name for the 
* file mapping object. The names of event, semaphore, mutex, waitable timer, 
* job, and file mapping objects share the same name space. Therefore, the 
* CreateFileMapping and OpenFileMapping functions fail if they specify a name
* that is in use by an object of another type.
* 
* To share memory that is not associated with a file, a process must use the 
* CreateFileMapping function and specify INVALID_HANDLE_VALUE as the hFile 
* parameter instead of an existing file handle. The corresponding file  
* mapping object accesses memory backed by the system paging file. You must  
* specify a size greater than zero when you specify an hFile of  
* INVALID_HANDLE_VALUE in a call to CreateFileMapping.
* 
* Processes that share files or memory must create file views by using the 
* MapViewOfFile or MapViewOfFileEx function. They must coordinate their  
* access using semaphores, mutexes, events, or some other mutual exclusion 
* technique.
* 
* This example demonstrates a named shared memory client, Local\HelloWorld, 
* that can access the string written to the shared memory by the first 
* process. The console displays the message "Message from the first process" 
* that is read from the file mapping created by the first process.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/1/2009 10:14 PM Hongye Sun Created
* * 3/8/2009 7:35 PM Jialiang Ge Reviewed
\***************************************************************************/

#pragma region Includes
#include "stdafx.h"

#include <windows.h>
#pragma endregion


#define BUFFER_SIZE		256 // 256 bytes

int _tmain()
{
	/////////////////////////////////////////////////////////////////////////
	// Try to open a named file mapping.
	//

	// In terminal services:   The name can have a "Global\" or "Local\"  
	// prefix to explicitly create the object in the global or session 
	// namespace. The remainder of the name can contain any character except 
	// the backslash character (\). For details, please refer to:
	// http://msdn.microsoft.com/en-us/library/aa366537.aspx
	TCHAR szMapFileName[] = _T("Local\\HelloWorld");

	// Open the named file mapping.
	HANDLE hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,	// Read/write access
		FALSE,					// Do not inherit the name
		szMapFileName			// Name of mapping object 
		);

	if (hMapFile == NULL) 
	{ 
		_tprintf(_T("Unable to open file mapping object w/err 0x%08lx\n"), 
			GetLastError());
		return 1;
	}
	_tprintf(_T("The file mapping object, %s, is opened.\n"), szMapFileName);


	/////////////////////////////////////////////////////////////////////////
	// Maps the view of the file mapping into the address space of the 
	// current process.
	// 

	// Create file view from the file mapping object.
	LPCTSTR pBuf = (LPTSTR) MapViewOfFile(
		hMapFile,				// Handle of the map object
		FILE_MAP_ALL_ACCESS,	// Read/write permission
		0,						// A high-order DWORD of the file offset 
								// where the view begins.
		0,						// A low-order DWORD of the file offset 
								// where the view is to begin. 
		BUFFER_SIZE				// The number of bytes of a file mapping 
								// to map to the view.
		);

	if (pBuf == NULL) 
	{ 
		_tprintf(_T("Unable to map view of file w/err 0x%08lx\n"), 
			GetLastError()); 
		CloseHandle(hMapFile);
		return 1;
	}
	_tprintf(_T("The file view is created.\n"));


	/////////////////////////////////////////////////////////////////////////
	// Show the message in shared memory.
	// 

	_tprintf(_T("The following message is read from the shared memory:\n") \
		_T("\"%s\"\n"), pBuf);


	/////////////////////////////////////////////////////////////////////////
	// Unmap the file view and close the file mapping objects.
	// 

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	return 0;
}

