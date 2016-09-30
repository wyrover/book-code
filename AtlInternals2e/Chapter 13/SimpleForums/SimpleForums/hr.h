//
// I just can't live without this one.
//

#if !defined( INC_HR_H_ )
#define INC_HR_H_

#define lengthof(rg) (sizeof(rg)/sizeof(*rg))

inline const char* StringFromError(char* szErr, long nSize, long nErr)
{
    _ASSERTE(szErr);
    *szErr = 0;
    DWORD cb = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, nErr, 0, szErr, nSize, 0);
    char szUnk[] = "<unknown>";

    if (!cb && nSize >= lengthof(szUnk)) lstrcpyA(szErr, szUnk);

    return szErr;
}

inline HRESULT TraceHR(const char* pszFile, long nLine, HRESULT hr)
{
    if (FAILED(hr)) {
        char szErr[128];
        char sz[_MAX_PATH + lengthof(szErr) + 64];
        wsprintfA(sz, "%s(%d) : error 0x%x: %s\n", pszFile, nLine, hr,
                  StringFromError(szErr, lengthof(szErr), hr));
        OutputDebugStringA(sz);
    }

    return hr;
}

#ifdef _DEBUG
    #define TRACEHR(_hr) TraceHR(__FILE__, __LINE__, _hr)
#else
    #define TRACEHR(_hr) _hr
#endif

#define HR(ex) { HRESULT _hr = ex; if(FAILED(_hr)) return TRACEHR(_hr), _hr; }

#define AS_HR(ex) { HRESULT _hr = ex; if(FAILED(_hr)) { TRACEHR(_hr); return HTTP_FAIL; } }

#endif
