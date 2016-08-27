#ifndef dns_query_h__
#define dns_query_h__

#include <windows.h>
#include <string>
#include <iostream>
#include <windns.h>
#include <Winhttp.h>


/**
使用winhttp的api从url中获取到域名
*/
std::wstring GetHostName(const std::wstring& url);

std::wstring dns_query();


#endif // dns_query_h__
