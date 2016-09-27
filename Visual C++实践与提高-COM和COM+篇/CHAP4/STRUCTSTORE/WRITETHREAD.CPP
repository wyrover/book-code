// WriteThread.cpp (EX26A)

#include "StdAfx.h"
#include "Thread.h"

int g_nIndent = 0;
const char* g_szBlanks = "                                          ";
const char* g_szRootStorageName = "\\direct.stg";


UINT WriteThreadProc(LPVOID pParam)
{
    USES_CONVERSION;
    LPSTORAGE pStgRoot = NULL;
    g_nIndent = 0;
    VERIFY(::StgCreateDocfile(T2COLE(g_szRootStorageName),
                              STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
                              0, &pStgRoot) == S_OK);
    ReadDirectory("\\", pStgRoot);
    pStgRoot->Release();
    AfxMessageBox("Write complete");
    return 0;
}

void ReadDirectory(const char* szPath, LPSTORAGE pStg)
{
    // recursive function
    USES_CONVERSION;
    WIN32_FIND_DATA fData;
    HANDLE h;
    char szNewPath[MAX_PATH];
    char szStorageName[100];
    char szStreamName[100];
    char szData[81];
    char* pch = NULL;
    LPSTORAGE pSubStg = NULL;
    LPSTREAM pStream = NULL;
    g_nIndent++;
    strcpy(szNewPath, szPath);
    strcat(szNewPath, "*.*");
    h = ::FindFirstFile(szNewPath, &fData);

    if (h == (HANDLE) 0xFFFFFFFF) return;  // can't find directory

    do {
        if (!strcmp(fData.cFileName, "..") ||
            !strcmp(fData.cFileName, ".")) continue;

        while ((pch = strchr(fData.cFileName, '!')) != NULL) {
            *pch = '|';
        }

        if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // It's a directory, so make a storage
            strcpy(szNewPath, szPath);
            strcat(szNewPath, fData.cFileName);
            strcat(szNewPath, "\\");
            strcpy(szStorageName, fData.cFileName);
            szStorageName[31] = '\0';    // limit imposed by OLE
            TRACE("%0.*sStorage = %s\n", (g_nIndent - 1) * 4,
                  g_szBlanks, szStorageName);
            VERIFY(pStg->CreateStorage(T2COLE(szStorageName),
                                       STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                       0, 0, &pSubStg) == S_OK);
            ASSERT(pSubStg != NULL);
            ReadDirectory(szNewPath, pSubStg);
            pSubStg->Release();
        } else {
            if ((pch = strrchr(fData.cFileName, '.')) != NULL) {
                if (!stricmp(pch, ".TXT")) {
                    // It's a text file, so make a stream
                    strcpy(szStreamName, fData.cFileName);
                    strcpy(szNewPath, szPath);
                    strcat(szNewPath, szStreamName);
                    szStreamName[32] = '\0'; // OLE max length
                    TRACE("%0.*sStream = %s\n", (g_nIndent - 1) * 4,
                          g_szBlanks, szNewPath);
                    CStdioFile file(szNewPath, CFile::modeRead);

                    // Ignore zero-length files
                    if (file.ReadString(szData, 80)) {
                        TRACE("%s\n", szData);
                        VERIFY(pStg->CreateStream(T2COLE(szStreamName),
                                                  STGM_CREATE | STGM_READWRITE |
                                                  STGM_SHARE_EXCLUSIVE,
                                                  0, 0, &pStream) == S_OK);
                        ASSERT(pStream != NULL);
                        // Include the null terminator in the stream
                        pStream->Write(szData, strlen(szData) + 1, NULL);
                        pStream->Release();
                    }
                }
            }
        }
    } while (::FindNextFile(h, &fData));

    g_nIndent--;
}
