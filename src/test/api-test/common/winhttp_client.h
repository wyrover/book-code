#ifndef winhttp_client_h__
#define winhttp_client_h__

#include <windows.h>
#include <Winhttp.h>

static const unsigned int INT_RETRYTIMES = 3;
static wchar_t *SZ_AGENT = L"WinHttpClient";
static const int INT_BUFFERSIZE = 10240;    // Initial 10 KB temporary buffer, double if it is not enough.

typedef struct WIN_HTTP_CLIENT {
    wchar_t m_proxy[MAX_PATH];
    wchar_t m_proxyUsername[50];
    wchar_t m_proxyPassword[50];

    wchar_t m_userAgent[50];
    DWORD m_dwLastError;
    BOOL m_requireValidSsl;

    unsigned int m_resolveTimeout;
    unsigned int m_connectTimeout;
    unsigned int m_sendTimeout;
    unsigned int m_receiveTimeout;

} WinHttp, *LPWinHttp;



void WinHttpInit();
void SetProxy(const wchar_t *proxyhost, const wchar_t *user, const wchar_t *passwd);
void SetUserAgent(const wchar_t *useragent);

int getLastError();
int SendHttpRequest(const wchar_t *verb, wchar_t *url, unsigned char *sendbuf, int sendbuflen, char *output, BOOL proxyFlah);

#endif // winhttp_client_h__
