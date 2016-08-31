#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/ie_proxy.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/ie_proxy", "[common/ie_proxy]")
{
    SECTION("testIsUserInAdminGroup", "[common/ie_proxy]") {
        cactus::CIEProxy proxyConfig;
        //net::E_proxy_type proxyType = net::eProxyType_http;
        cactus::E_proxy_type proxyType = cactus::eProxyType_all;
        URL_COMPONENTS url_component = { 0 };
        std::wstring strUrl = L"http://msdn.microsoft.com/en-us/library/windows/desktop/aa384376(v=vs.85).aspx";
        url_component.dwStructSize = sizeof(URL_COMPONENTS);
        url_component.dwHostNameLength = (DWORD) - 1;
        ::WinHttpCrackUrl(strUrl.c_str(), (DWORD)strUrl.length(), 0, &url_component);
        std::wstring strHostName = std::wstring(url_component.lpszHostName, url_component.dwHostNameLength);
        //strHostName = L"127.0.0.1";
        strHostName = L"localhost";
        std::wstring strProxy = proxyConfig.GetIEProxy(strHostName.c_str(), proxyType);
        std::wcout << strProxy << std::endl;
    }
}

