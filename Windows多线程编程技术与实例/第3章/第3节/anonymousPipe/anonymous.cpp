#include "stdio.h"
#include "iostream.h"
#include "windows.h"
#include "process.h"

#include "tchar.h"

const int  BUF_SIZE=100;

void main()
{
	HANDLE					PipeReadHandle;
	HANDLE					PipeWriteHandle;
	PROCESS_INFORMATION		ProcessInfo;
	SECURITY_ATTRIBUTES		SecurityAttributes;
	STARTUPINFO				StartupInfo;
	BOOL					Success;

	//--------------------------------------------------------------------------
	//	Zero the structures.
	//--------------------------------------------------------------------------
	ZeroMemory( &StartupInfo,			sizeof( StartupInfo ));
	ZeroMemory( &ProcessInfo,			sizeof( ProcessInfo ));
	ZeroMemory( &SecurityAttributes,	sizeof( SecurityAttributes ));

	//--------------------------------------------------------------------------
	//	Create a pipe for the child's STDOUT.
	//--------------------------------------------------------------------------
	SecurityAttributes.nLength              = sizeof(SECURITY_ATTRIBUTES);
	SecurityAttributes.bInheritHandle       = TRUE;
	SecurityAttributes.lpSecurityDescriptor = NULL;

	Success = CreatePipe
	(
		&PipeReadHandle,		// address of variable for read handle
		&PipeWriteHandle,		// address of variable for write handle
		&SecurityAttributes,	// pointer to security attributes
		0						// number of bytes reserved for pipe (use default size)
	);

	if ( !Success )
	{
	//	ShowLastError(_T("Error creating pipe"));
		cout<<"error creating pipe"<<endl;
		return;
	}	

	//--------------------------------------------------------------------------
	//	Set up members of STARTUPINFO structure.
	//--------------------------------------------------------------------------
	StartupInfo.cb           = sizeof(STARTUPINFO);
	StartupInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	StartupInfo.wShowWindow  = SW_HIDE;
	StartupInfo.hStdOutput   = PipeWriteHandle;
	StartupInfo.hStdError    = PipeWriteHandle;

	//----------------------------------------------------------------------------
	//	Create the child process.
	//----------------------------------------------------------------------------
	Success = CreateProcess
	( 
		NULL,					// pointer to name of executable module
		"sunProcess.exe",	// command line 
		NULL,					// pointer to process security attributes 
		NULL,					// pointer to thread security attributes (use primary thread security attributes)
		TRUE,					// inherit handles
		0,						// creation flags
		NULL,					// pointer to new environment block (use parent's)
		NULL,	// pointer to current directory name
		&StartupInfo,			// pointer to STARTUPINFO
		&ProcessInfo			// pointer to PROCESS_INFORMATION
	);                 

	if ( !Success )
	{
		// ShowLastError(_T("Error creating process"));
		cout<<"Error creating process"<<endl;
		return;
	}

	DWORD	BytesLeftThisMessage = 0;
	DWORD	NumBytesRead;
	TCHAR	PipeData[BUF_SIZE]; 
	DWORD	TotalBytesAvailable = 0;

	Sleep(200);
	Success=ReadFile
			(
				PipeReadHandle,		// handle to pipe to copy from 
				PipeData,			// address of buffer that receives data
				BUF_SIZE-1,		// number of bytes to read
				&NumBytesRead,		// address of number of bytes read
				NULL				// address of structure for data for overlapped I/O
			);
	while (Success)
	{
		
		PipeData[NumBytesRead] = '\0';
		
		cout<<PipeData<<endl;

	
		//判断子进程是否已结束
		if ( WaitForSingleObject(ProcessInfo.hProcess, 0) == WAIT_OBJECT_0)
		{
			cout<<"sunProcess is end"<<endl;
			break;
		}
		Sleep(100);
		Success=ReadFile
		(
			PipeReadHandle,		// handle to pipe to copy from 
			PipeData,			// address of buffer that receives data
			BUF_SIZE-1,		// number of bytes to read
			&NumBytesRead,		// address of number of bytes read
			NULL				// address of structure for data for overlapped I/O
		);

	}
	 CloseHandle(ProcessInfo.hThread);
	 CloseHandle(ProcessInfo.hProcess);
	 CloseHandle(PipeReadHandle);
	 CloseHandle(PipeWriteHandle);
	 cout<<"end"<<endl;


}