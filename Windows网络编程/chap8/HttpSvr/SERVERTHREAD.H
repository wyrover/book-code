//Serverthread.h

class CSocketAddress;
class CMyBlockSocket;
class CMyHttpBlockSocket;
class CMyBlockSocketException;

extern volatile int g_nConnection;//
extern volatile BOOL g_bListening;//
extern CString g_strDirect;//
extern CString g_strIPServer;//
extern volatile UINT g_nPortServer;//
extern CString g_strDefault;//

extern UINT ServerThreadProc(LPVOID pParam);

extern void LogBlockingSocketException(LPVOID pParam, char* pch, CMyBlockSocketException* pe);

extern void LogRequest(LPVOID pParam,char *pch,CSocketAddress sa);