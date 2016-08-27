#include "file_crc32.h"

#include "crc32.h"
#include <Windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <atlstr.h>

#pragma comment(lib, "shlwapi.lib")

bool GetFileCRC32(const TCHAR* pszFileName, unsigned& uCrc32)
{
    bool    bRetCode    = false;
    HANDLE  hFileMap    = NULL;
    HANDLE  hFile       = INVALID_HANDLE_VALUE;
    void*   pView       = NULL;
    DWORD   dwFileSize  = 0;

    if (NULL == pszFileName)
        goto Exit0;

    uCrc32 = 0;

    //Check file exist
    if (!PathFileExists(pszFileName))
        goto Exit0;

    //Create file
    hFile = CreateFile(pszFileName,
                       GENERIC_READ,
                       FILE_SHARE_READ,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL
                      );

    if (INVALID_HANDLE_VALUE == hFile)
        goto Exit0;

    //Get size of file
    dwFileSize = GetFileSize(hFile, NULL);

    if (INVALID_FILE_SIZE == dwFileSize)
        goto Exit0;

    //Create file mapping
    hFileMap = CreateFileMapping(hFile,
                                 NULL,
                                 PAGE_READONLY,
                                 0,
                                 dwFileSize,
                                 NULL
                                );

    if (NULL == hFileMap)
        goto Exit0;

    pView = MapViewOfFile(hFileMap,
                          FILE_MAP_READ,
                          0,
                          0,
                          dwFileSize
                         );

    if (NULL == pView)
        goto Exit0;

    //Compute the CRC32 value of file
    uCrc32 = CRC32(uCrc32, pView, dwFileSize);
    bRetCode = true;
Exit0:

    if (NULL != pView) {
        UnmapViewOfFile(pView);
        pView = NULL;
    }

    if (NULL != hFileMap) {
        CloseHandle(hFileMap);
        hFileMap = NULL;
    }

    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }

    return bRetCode;
}

const int cnReadBufSize = 4096;

bool GetFileCRC32Ex(const TCHAR* pszFileName, unsigned& uCrc32)
{
    bool        bResult     = false;
    FILE*       pFile       = NULL;
    unsigned    uReadSize   = 0;
    char        szReadBuf[cnReadBufSize]    = {0};

    if (NULL == pszFileName)
        goto Exit0;

    //Open file
    pFile = fopen(CT2A(pszFileName), "rb");

    if (NULL == pFile)
        goto Exit0;

    uCrc32 = 0;

    while (true) {
        //Read
        uReadSize = (unsigned)fread(szReadBuf,
                                    sizeof(char),
                                    cnReadBufSize,
                                    pFile
                                   );

        if (0 == uReadSize)
            break;

        uCrc32 += CRC32(uCrc32, szReadBuf, uReadSize);
    }

    bResult = true;
Exit0:

    if (NULL != pFile) {
        fclose(pFile);
        pFile = NULL;
    }

    return bResult;
}