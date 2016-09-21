#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <time.h>
#include <io.h>

using namespace std;



DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    LPCTSTR pDirtory = (TCHAR*)lpParam;
    BOOL bRet = FALSE;
    BYTE Buffer[1024] = { 0 };
    FILE_NOTIFY_INFORMATION *pBuffer = (FILE_NOTIFY_INFORMATION *)Buffer;
    DWORD BytesReturned = 0;
    HANDLE hFile = CreateFile(pDirtory,
                              FILE_LIST_DIRECTORY,
                              FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
                              NULL,
                              OPEN_EXISTING,
                              FILE_FLAG_BACKUP_SEMANTICS,
                              NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        return 1;
    }

    printf("monitor... \r\n");
    fflush(stdout);

    while (TRUE) {
        ZeroMemory(Buffer, 1024);
        // 子线程一直在读取目录的改变
        // 调用系统 ReadDirectoryChangesW API
        bRet = ReadDirectoryChangesW(hFile,
                                     &Buffer,
                                     sizeof(Buffer),
                                     TRUE,
                                     FILE_NOTIFY_CHANGE_FILE_NAME |  // 修改文件名
                                     FILE_NOTIFY_CHANGE_ATTRIBUTES | // 修改文件属性
                                     FILE_NOTIFY_CHANGE_LAST_WRITE |
                                     FILE_NOTIFY_CHANGE_DIR_NAME, // 最后一次写入
                                     &BytesReturned,
                                     NULL, NULL);

        if (bRet == TRUE) {
            time_t ChangeTime;
            time(&ChangeTime);
            

            switch (pBuffer->Action) {
            // 添加
            case FILE_ACTION_ADDED: {
                //printf("添加 : %s\r\n", szFileName);
				std::wcout << ctime(&ChangeTime) << L" 添加:" << pBuffer->FileName << L"\n" << flush;
                break;
            }

            // 删除
            case FILE_ACTION_REMOVED: {
                //printf("删除 : %s\r\n", szFileName);
                std::wcout << ctime(&ChangeTime) << L" 删除:" << pBuffer->FileName << L"\n" << flush;
                break;
            }

            // 修改
            case FILE_ACTION_MODIFIED: {
                // 修改为文件夹
                _wfinddata_t file;
				std::wstring sdirfile;
                sdirfile = std::wstring(pDirtory);

                if (sdirfile.find_last_of(L"\\") != sdirfile.length()) {
					sdirfile.append(pDirtory);
					sdirfile.append(L"\\");
					sdirfile.append(pBuffer->FileName);
					
                } else {
					sdirfile.append(pDirtory);
					
					sdirfile.append(pBuffer->FileName);
					
                }

                long lf;

                if ((lf = _wfindfirst(sdirfile.c_str(), &file)) == -1l) {
					std::wcout << L" 目录不存在:" << sdirfile.c_str() << L"\n" << flush;
                    return 1;
                }

                if ((file.attrib & _A_SUBDIR) == 0) {
                    //printf("修改 : %s\r\n", szFileName);
                    std::wcout << ctime(&ChangeTime) << L" 修改:" << pBuffer->FileName << L"\n" << flush;
                }

                break;
            }

            // 重命名
            case FILE_ACTION_RENAMED_OLD_NAME: {
                //printf("重命名 : %s", szFileName);
                std::wcout << ctime(&ChangeTime) << L" 重命名:" << pBuffer->FileName;

                if (pBuffer->NextEntryOffset != 0) {
                    FILE_NOTIFY_INFORMATION *tmpBuffer = (FILE_NOTIFY_INFORMATION *)((DWORD)pBuffer + pBuffer->NextEntryOffset);

                    switch (tmpBuffer->Action) {
                    case FILE_ACTION_RENAMED_NEW_NAME: {                        
                        //printf("->  : %s \r\n", szFileName);
						std::wcout << L"->" << tmpBuffer->FileName << L"\n" << flush;
                        break;
                    }
                    }
                }

                break;
            }

            case FILE_ACTION_RENAMED_NEW_NAME: {
                //printf("重命名 (new) : %s\r\n", szFileName);
				std::wcout << ctime(&ChangeTime) << L" 重命名 (new):" << pBuffer->FileName << L"\n" << flush;
            }
            }
        }

        fflush(stdout);
    }

    CloseHandle(hFile);
    return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	_tsetlocale(LC_ALL, _T(""));

    if (argc == 1) {
        return 0;
    }

    HANDLE hThread = CreateThread(NULL, 0, ThreadProc, argv[1], 0, NULL);

    if (hThread == NULL) {
        return -1;
    }

    // 等待线程结束
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}