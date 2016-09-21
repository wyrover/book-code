// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

char usage[] = ""
"notify - print a name of changed object if something is is changed in a directory subtree.\n"
"version 2.00, 2011-06-08\n"
"(C) Dmitry Koterov (dmitry@koterov.ru)\n"
"dkLab RealSync project, http://en.dklab.ru/lib/dklab_realsync/\n\n"
"Usage:\n"
"  notify.exe <any-path-to-watch>\n"
;

using namespace std;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	_tsetlocale(LC_ALL, _T(""));
	if (!argv[1] || !*argv[1]) {
		fprintf(stderr, "%s", usage);
		ExitProcess(1); 
	}
	char* dir = argv[1];

	HANDLE hDir = CreateFile( 
		dir,                                // pointer to the file name
		FILE_LIST_DIRECTORY,                // access (read/write) mode
		// Share mode MUST be the following to avoid problems with renames via Explorer!
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
		NULL,                               // security descriptor
		OPEN_EXISTING,                      // how to create
		FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
		NULL                                // file with attributes to copy
		);
	if (hDir == INVALID_HANDLE_VALUE) {
		int lastError = GetLastError();
		char error[1024];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lastError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), error, sizeof(error), 0);
		fprintf(stderr, "ERROR: Cannot open the directory \"%s\":\n%s\n", dir, error);
		ExitProcess(GetLastError()); 
	}

	TCHAR szBuffer[1024 * 128];
	DWORD BytesReturned;
	while (ReadDirectoryChangesW(
		hDir,                          // handle to directory
		&szBuffer,                       // read results buffer
		sizeof(szBuffer),                // length of buffer
		TRUE,                          // monitoring option
		FILE_NOTIFY_CHANGE_SECURITY |
		FILE_NOTIFY_CHANGE_CREATION |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_FILE_NAME,  // filter conditions
		&BytesReturned,                // bytes returned
		NULL,                          // overlapped buffer
		NULL                           // completion routine
		)
		) {
			DWORD dwOffset = 0;
			FILE_NOTIFY_INFORMATION* pInfo = NULL;
			do {
				// Get a pointer to the first change record...
				pInfo = (FILE_NOTIFY_INFORMATION*)&szBuffer[dwOffset];

				char *action = "*";
				switch (pInfo->Action) {
				case FILE_ACTION_ADDED: action = "A"; break; 
				case FILE_ACTION_REMOVED: action = "D"; break; 
				case FILE_ACTION_MODIFIED: action = "M"; break; 
				case FILE_ACTION_RENAMED_OLD_NAME: action = "O"; break; 
				case FILE_ACTION_RENAMED_NEW_NAME: action = "N"; break;
				}

				// ReadDirectoryChangesW processes filenames in Unicode. We will convert them to a TCHAR format...
				TCHAR szFileName[MAX_PATH] = {0};
				WideCharToMultiByte(CP_ACP, NULL, pInfo->FileName, pInfo->FileNameLength, szFileName, sizeof(szFileName) / sizeof(TCHAR), NULL, NULL);
				szFileName[pInfo->FileNameLength / 2] = 0;

				printf("%s %s\n", action, szFileName);

				// More than one change may happen at the same time. Load the next change and continue...
				dwOffset += pInfo->NextEntryOffset;
			} while (pInfo->NextEntryOffset != 0);
			printf("-\n");
			fflush(stdout);
	}

	return 0;
}

