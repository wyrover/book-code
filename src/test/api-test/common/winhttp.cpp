#include "winhttp.h"

WinHttp::WinHttp()
{
    //WinHttpOpen
    hSession        = NULL;
    userAgent       = NULL;
    accessType      = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
    proxyName       = WINHTTP_NO_PROXY_NAME;
    proxyBypass     = WINHTTP_NO_PROXY_BYPASS;
    //WinHttpConnect
    hConnect        = NULL;
    url             = NULL;
    serverPort      = INTERNET_DEFAULT_HTTPS_PORT;
    reserved        = 0;
    //WinHttpOpenRequest
    hRequest        = NULL;
    httpMethod      = L"GET";
    objectName      = NULL;
    version         = L"HTTP/1.1";
    referrer        = WINHTTP_NO_REFERER;
    acceptTypes     = WINHTTP_DEFAULT_ACCEPT_TYPES;
    flags           = WINHTTP_FLAG_SECURE;
    //WinHttpSendRequest
    bResults        = FALSE;
    optional        = WINHTTP_NO_REQUEST_DATA;
    optionalLength  = NULL;
    context         = NULL;
    isScannd        = false;
}

WinHttp::~WinHttp()
{
    /*if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);*/
}

void WinHttp::setAgent(LPCWSTR userAgent)
{
    this->userAgent = userAgent;
}

void WinHttp::setProxy(LPCWSTR proxyName, DWORD accessType, LPCWSTR proxyBypass)
{
    this->proxyName     = proxyName;
    this->accessType    = accessType;
    this->proxyBypass   = proxyBypass;
}

void WinHttp::setTargetAddress(LPCWSTR url, INTERNET_PORT serverPort)
{
    this->url           = url;
    this->serverPort    = serverPort;
}

void WinHttp::setMethod(LPCWSTR httpMethod)
{
    this->httpMethod = httpMethod;
}

void WinHttp::setHttpVersion(LPCWSTR version)
{
    this->version = version;
}

void WinHttp::setReferrer(LPCWSTR referrer)
{
    this->referrer = referrer;
}

void WinHttp::setTargetObjectName(LPCWSTR objectName)
{
    this->objectName = objectName;
}

void WinHttp::setAcceptTypes(LPCWSTR acceptTypes)
{
    this->acceptTypes = acceptTypes;
}

void WinHttp::addHeader(const char * str)
{
    header += str;
}

void WinHttp::addHeaderln(const char * str)
{
    header += str;
    header += "\r\n";
}

void WinHttp::setOptional(LPVOID optional, DWORD optionalLength)
{
    this->optional          = optional;
    this->optionalLength    = optionalLength;
}

void WinHttp::setConnectFlags(DWORD flag)
{
    this->flags = flag;
}

int WinHttp::scanned()
{
    request.clear();
    requestHeader.clear();
    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(userAgent,
                           accessType,
                           proxyName,
                           proxyBypass, 0);

    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, url,
                                  serverPort, reserved);
    else {
        return (int)hSession;
        isScannd = false;
    }

    isScannd = true;
    return 0;
}

bool WinHttp::sendRequest()
{
    if (!isScannd)
        scanned();

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, httpMethod, objectName,
                                      version, referrer,
                                      &acceptTypes,
                                      flags);
    else
        return false;

    // Send a request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
                                      NULL, header.length(),
                                      optional, optionalLength,
                                      header.length() + optionalLength, context);

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Keep checking for data until there is nothing left.
    if (bResults) {
        DWORD dwSize = 0;

        do {
            // Check for available data.
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable.\n",
                       GetLastError());

            // Allocate space for the buffer.
            LPSTR pszOutBuffer = new char[dwSize + 1];

            if (!pszOutBuffer) {
                printf("Out of memory\n");
                dwSize = 0;
            } else {
                // Read the data.
                ZeroMemory(pszOutBuffer, dwSize + 1);
                DWORD dwDownloaded = 0;

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                                     dwSize, &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else
                    request += pszOutBuffer;

                // Free the memory allocated to the buffer.
                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);
    }

    if (!bResults)
        printf("Error %d has occurred.\n", GetLastError());

    return true;
}


void WinHttp::printTxt()
{
    //FileManager fm("test.txt");
    //fm.deleteFile();
    //for (unsigned int index = 0; index < request.length(); ++index) {
    //  std::cout << request[index];
    //}
    //fm.saveString(request);
}

void WinHttp::printHeader()
{
    for (int index = 0; index < requestHeader.length(); ++index) {
        std::cout << requestHeader[index];
    }
}