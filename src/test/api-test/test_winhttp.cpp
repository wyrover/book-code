#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/winhttp.h"
#include "common/http_request.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/winhttp", "[common/winhttp]")
{
    SECTION("testWinHttpCrackUrl", "[common/winhttp]") {
        URL_COMPONENTS stUrlComp;
        LPCWSTR pwszUrl1 = L"http://172.17.195.146/hfs_up/a.tmp";
        DWORD dwUrlLen = 0;
        ZeroMemory(&stUrlComp, sizeof(URL_COMPONENTS));
        stUrlComp.dwStructSize = sizeof(URL_COMPONENTS);
        wchar_t lpszHostName[MAX_PATH] = { 0 };
        wchar_t lpszPath[MAX_PATH] = { 0 };
        wchar_t lpszExtra[MAX_PATH] = { 0 };
        stUrlComp.lpszHostName = lpszHostName;
        stUrlComp.dwHostNameLength  = MAX_PATH;
        stUrlComp.lpszUrlPath = lpszPath;
        stUrlComp.dwUrlPathLength = MAX_PATH;
        stUrlComp.lpszExtraInfo = lpszExtra;
        stUrlComp.dwExtraInfoLength = MAX_PATH;

        if (!::WinHttpCrackUrl(pwszUrl1, (DWORD)(LONG_PTR)wcslen(pwszUrl1), ICU_ESCAPE, &stUrlComp)) {
            ::printf("Error %u in WinHttpCrackUrl.\n", GetLastError());
        } else {
            ::printf("HostName : %S\n", lpszHostName);
            ::printf("Path : %S\n", lpszPath);
            ::printf("ExtraInfo : %S\n", lpszExtra);
        }
    }
    SECTION("testWinHttpOpen", "[common/winhttp]") {
        LPSTR pszData = "WinHttpWriteData Example";
        DWORD dwBytesWritten = 0;
        BOOL  bResults = FALSE;
        HINTERNET hSession = NULL,
                  hConnect = NULL,
                  hRequest = NULL;
        // Use WinHttpOpen to obtain a session handle.
        hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
                               WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                               WINHTTP_NO_PROXY_NAME,
                               WINHTTP_NO_PROXY_BYPASS, 0);

        // Specify an HTTP server.
        if (hSession)
            hConnect = WinHttpConnect(hSession, L"www.wingtiptoys.com",
                                      INTERNET_DEFAULT_HTTP_PORT, 0);

        // Create an HTTP Request handle.
        if (hConnect)
            hRequest = WinHttpOpenRequest(hConnect, L"GET",
                                          L"/writetst.txt",
                                          NULL, WINHTTP_NO_REFERER,
                                          WINHTTP_DEFAULT_ACCEPT_TYPES,
                                          0);

        // Send a Request.
        if (hRequest)
            bResults = WinHttpSendRequest(hRequest,
                                          WINHTTP_NO_ADDITIONAL_HEADERS,
                                          0, WINHTTP_NO_REQUEST_DATA, 0,
                                          (DWORD)strlen(pszData), 0);

        // Write data to the server.
        if (bResults)
            bResults = WinHttpWriteData(hRequest, pszData,
                                        (DWORD)strlen(pszData),
                                        &dwBytesWritten);

        // End the request.
        if (bResults)
            bResults = WinHttpReceiveResponse(hRequest, NULL);

        // Report any errors.
        if (!bResults)
            printf("Error %d has occurred.\n", GetLastError());

        // Close any open handles.
        if (hRequest) WinHttpCloseHandle(hRequest);

        if (hConnect) WinHttpCloseHandle(hConnect);

        if (hSession) WinHttpCloseHandle(hSession);
    }
    SECTION("testWinHttp", "[common/winhttp]") {
        WinHttp wp;
        wp.setTargetAddress(L"www.wingtiptoys.com", INTERNET_DEFAULT_HTTP_PORT);
        wp.setConnectFlags(NULL);
        wp.scanned();
        wp.sendRequest();
        wp.printTxt();
        wp.printHeader();
    }
    SECTION("testHttpRequest", "[common/HttpRequest]") {
        HttpRequest test;
        test.setUrl("graph.facebook.com");
        test.addHeaderln(
            //  "GET / HTTP/1.1\r\n"
            "GET /v2.5/100000532497017?access_token=ACCESS_TOKEN HTTP/1.1\r\n"
            "Host: graph.facebook.com\r\n"
            "message: The Message\r\n"
            "id: 100000532497017\r\n"
            //  "Connection: close\r\n"
        );
        test.setPort("http");
        test.setFamily(AF_UNSPEC);
        test.printIP();
        test.scand();
    }
}

