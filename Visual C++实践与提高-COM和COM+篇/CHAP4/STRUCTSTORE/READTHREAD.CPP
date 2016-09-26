// ReadThread.cpp (EX26A)

#include "StdAfx.h"
#include "Thread.h"

UINT ReadThreadProc(LPVOID pParam)
{
    USES_CONVERSION;
    LPSTORAGE pStgRoot = NULL;
    // doesn't work without STGM_SHARE_EXCLUSIVE
    g_nIndent = 0;

    if (::StgOpenStorage(T2COLE(g_szRootStorageName), NULL,
                         STGM_READ | STGM_SHARE_EXCLUSIVE,
                         NULL, 0, &pStgRoot) == S_OK) {
        ASSERT(pStgRoot != NULL);
        ReadStorage(pStgRoot);
        pStgRoot->Release();
    } else {
        AfxMessageBox("Storage file not available or not readable");
    }

    AfxMessageBox("Read complete");
    return 0;
}

void ReadStorage(LPSTORAGE pStg)
// reads one storage -- recursive calls for substorages
{
    USES_CONVERSION;
    LPSTORAGE pSubStg = NULL;
    LPSTREAM pStream = NULL;
    LPENUMSTATSTG pEnum = NULL;
    LPMALLOC pMalloc = NULL; // for freeing statstg
    STATSTG statstg;
    ULONG nLength;
    BYTE buffer[101];
    g_nIndent++;
    ::CoGetMalloc(MEMCTX_TASK, &pMalloc); // assumes AfxOleInit
    //  was called
    VERIFY(pStg->EnumElements(0, NULL, 0, &pEnum) == S_OK);

    while (pEnum->Next(1, &statstg, NULL) == S_OK) {
        if (statstg.type == STGTY_STORAGE) {
            VERIFY(pStg->OpenStorage(statstg.pwcsName, NULL,
                                     STGM_READ | STGM_SHARE_EXCLUSIVE,
                                     NULL, 0, &pSubStg) == S_OK);
            ASSERT(pSubStg != NULL);
            TRACE("%0.*sStorage = %s\n", (g_nIndent - 1) * 4,
                  g_szBlanks, OLE2CT(statstg.pwcsName));
            ReadStorage(pSubStg);
            pSubStg->Release();
        } else if (statstg.type == STGTY_STREAM) {
            VERIFY(pStg->OpenStream(statstg.pwcsName, NULL,
                                    STGM_READ | STGM_SHARE_EXCLUSIVE,
                                    0, &pStream) == S_OK);
            ASSERT(pStream != NULL);
            TRACE("%0.*sStream = %s\n", (g_nIndent - 1) * 4,
                  g_szBlanks, OLE2CT(statstg.pwcsName));
            pStream->Read(buffer, 100, &nLength);
            buffer[nLength] = '\0';
            TRACE("%s\n", buffer);
            pStream->Release();
        } else {
            ASSERT(FALSE);  // LockBytes?
        }

        pMalloc->Free(statstg.pwcsName); // avoids memory leaks
    }

    pMalloc->Release();
    pEnum->Release();
    g_nIndent--;
}

