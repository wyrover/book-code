#ifndef winhttp_h__
#define winhttp_h__

#include <Windows.h>
#include <winhttp.h>
#include <iostream>

class WinHttp
{
public:
    WinHttp();
    ~WinHttp();
    void setAgent(LPCWSTR userAgent);
    void setProxy(LPCWSTR proxyName, DWORD accessType, LPCWSTR proxyBypass);
    void setTargetAddress(LPCWSTR url, INTERNET_PORT serverPort);
    void setMethod(LPCWSTR httpMethod);
    void setHttpVersion(LPCWSTR version);
    void setReferrer(LPCWSTR referrer);
    void setTargetObjectName(LPCWSTR objectName);
    void setAcceptTypes(LPCWSTR acceptTypes);
    void addHeader(const char* str);
    void addHeaderln(const char* str);
    void setOptional(LPVOID optional, DWORD optionalLength);
    void setConnectFlags(DWORD flag);
    int scanned();
    bool sendRequest();
    void printTxt();
    void printHeader();
private:
    //WinHttpOpen
    HINTERNET hSession;
    LPCWSTR userAgent;
    DWORD accessType;
    LPCWSTR proxyName;
    LPCWSTR proxyBypass;

    //WinHttpConnect
    HINTERNET hConnect;
    LPCWSTR url;
    INTERNET_PORT serverPort;
    DWORD reserved;

    //WinHttpOpenRequest
    HINTERNET hRequest;
    LPCWSTR httpMethod;
    LPCWSTR objectName;
    LPCWSTR version;
    LPCWSTR referrer;
    LPCWSTR acceptTypes;
    DWORD flags;

    //WinHttpSendRequest
    BOOL  bResults;
    std::string header;
    LPVOID optional;
    DWORD optionalLength;
    DWORD_PTR  context;

    bool isScannd;
    std::string request;
    std::string requestHeader;
};

#endif // winhttp_h__
