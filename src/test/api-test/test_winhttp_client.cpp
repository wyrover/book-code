#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/winhttp_client.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/winhttp_client", "[common/winhttp_client]")
{
    SECTION("testWinHttpInit", "[common/winhttp_client]") {
        wchar_t *pszServer = L"http://192.168.1.33/jsbin/server.php";
        wchar_t *server2 = L"http://192.168.1.33/test.php";
        wchar_t *httpsurl = L"https://www.google.com/";
        char recv[10240] = {0};
        char *pszPostData = "teststr=Hello+world&testval=42";
        int recvlen = 0;
        wchar_t size[50] = L"";
        WinHttpInit();
        recvlen = SendHttpRequest(L"POST", pszServer, (UCHAR*)pszPostData, strlen(pszPostData) + 1, recv, FALSE);
        printf("%s\n", recv);
        recvlen = SendHttpRequest(L"POST", server2, (UCHAR*)pszPostData, strlen(pszPostData) + 1, recv, FALSE);
        printf("%s\n", recv);
        //proxy test
        SetProxy(L"192.168.1.1:3128", L"", L"");
        recvlen = SendHttpRequest(L"POST", server2, (UCHAR*)pszPostData, strlen(pszPostData) + 1, recv, TRUE);
        printf("%s\n", recv);
        recvlen = SendHttpRequest(L"POST", httpsurl, (UCHAR*)pszPostData, strlen(pszPostData) + 1, recv, FALSE);
        printf("%s\n", recv);
    }
}

