// Thread.h
UINT WriteThreadProc(LPVOID pParam);
UINT ReadThreadProc(LPVOID pParam);
void ReadDirectory(const char* szPath, LPSTORAGE pStg);
void ReadStorage(LPSTORAGE pStg);
