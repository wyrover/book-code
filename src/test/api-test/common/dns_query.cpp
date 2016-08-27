#include "dns_query.h"

std::wstring GetHostName(const std::wstring& url)
{
    URL_COMPONENTS url_component;
    ::ZeroMemory(&url_component, sizeof(URL_COMPONENTS));
    url_component.dwStructSize = sizeof(URL_COMPONENTS);
    wchar_t host_name[1024] = { 0 };
    wchar_t path[1024] = { 0 };
    url_component.lpszHostName = host_name;
    url_component.dwHostNameLength = 1024;
    url_component.lpszUrlPath = path;
    url_component.dwUrlPathLength = 1024;

    if (!::WinHttpCrackUrl(url.c_str(), url.length(), ICU_ESCAPE, &url_component)) {
        return L"";
    } else {
        return host_name;
    }
}

std::wstring dns_query()
{
    std::wstring dns_result;
    std::wstring hostname = GetHostName(L"http://www.baidu.com");
    PDNS_RECORD query_result_list = NULL;
    DNS_STATUS nStatus = ::DnsQuery(hostname.c_str(), DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE, NULL, &query_result_list, NULL);

    if (nStatus != 0) {
        //DNS_ERROR_RCODE_NAME_ERROR
    } else {
        int index = 1;

        for (PDNS_RECORD ptr = query_result_list; ptr != NULL; ptr = ptr->pNext) {
            if (ptr->wType == DNS_TYPE_A) {
                wchar_t ip[256] = L"";
                ::_snwprintf_s(ip, _countof(ip), 255, L"%d=%d.%d.%d.%d",
                               index++,
                               (ptr->Data.A.IpAddress) & 0xff,
                               ((ptr->Data.A.IpAddress) & 0xff00) >> 8,
                               ((ptr->Data.A.IpAddress) & 0xff0000) >> 16,
                               ((ptr->Data.A.IpAddress) & 0xff000000) >> 24);

                if (!dns_result.empty()) {
                    dns_result += L";";
                }

                dns_result += ip;
            }
        }
    }

    if (query_result_list != NULL) {
        ::DnsFree(query_result_list, DnsFreeRecordList);
    }

    return dns_result;
}
