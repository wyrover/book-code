// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinHttpClient.h"

void SimpleGetTest(void)
{
    // Set URL.
    WinHttpClient client(L"http://www.codeproject.com/");

    // Send http request, a GET request by default.
    client.SendHttpRequest();

    // The response header.
    wstring httpResponseHeader = client.GetResponseHeader();

    // The response content.
    wstring httpResponseContent = client.GetResponseContent();
}

void SimplePostTest(void)
{
    WinHttpClient client(L"http://www.codeproject.com/");

    // Set post data.
    string data = "title=A_NEW_THREAD&content=This_is_a_new_thread.";
    client.SetAdditionalDataToSend((BYTE *)data.c_str(), data.size());

    // Set request headers.
    wchar_t szSize[50] = L"";
    swprintf_s(szSize, L"%d", data.size());
    wstring headers = L"Content-Length: ";
    headers += szSize;
    headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
    client.SetAdditionalRequestHeaders(headers);

    // Send http post request.
    client.SendHttpRequest(L"POST");

    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

// Progress - finished percentage.
bool ProgressProc(double progress)
{
    wprintf(L"Current progress: %-.1f%%\r\n", progress);
    return true;
}

void ProgressTest(void)
{
    // Set URL and call back function.
    WinHttpClient client(L"http://www.codeproject.com/", ProgressProc);
    client.SendHttpRequest();
    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

void UserAgentTest(void)
{
    WinHttpClient client(L"http://www.codeproject.com/");

    // Set the user agent to the same as IE8.
    client.SetUserAgent(L"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; CIBA; MS-RTC LM 8)");

    client.SendHttpRequest();
    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

void ProxyTest(void)
{
    WinHttpClient client(L"http://www.codeproject.com/");

    // Set the proxy to 192.168.0.1 with port 8080.
    client.SetProxy(L"192.168.0.1:8080");

    client.SendHttpRequest();
    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

void CookiesTest(void)
{
    WinHttpClient client(L"http://www.codeproject.com/");

    // Set the cookies to send.
    client.SetAdditionalRequestCookies(L"username=jack");

    client.SendHttpRequest();

    // Get the response cookies.
    wstring httpResponseCookies = client.GetResponseCookies();

    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

void HttpsTest(void)
{
    WinHttpClient client(L"https://www.google.com/");

    // Accept any certificate while performing HTTPS request.
    client.SetRequireValidSslCertificates(false);

    // Get the response cookies.
    wstring httpResponseCookies = client.GetResponseCookies();

    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();
}

void MultipleRequestsTest(void)
{
    WinHttpClient client(L"http://www.google.com/");
     
    client.SendHttpRequest();
    wstring httpResponseHeader = client.GetResponseHeader();
    wstring httpResponseContent = client.GetResponseContent();

    // Update the url.
    client.UpdateUrl(L"http://www.microsoft.com/");
    client.SendHttpRequest();
    httpResponseHeader = client.GetResponseHeader();
    httpResponseContent = client.GetResponseContent();
}

void CompleteTest(void)
{
    // 1. Get the initial cookie.
    WinHttpClient getClient(L"http://www.codeproject.com/script/Membership/LogOn.aspx");
    getClient.SetAdditionalRequestHeaders(L"Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\nAccept-Language: en-us\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; QQPinyin 730; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; CIBA; MS-RTC LM 8)\r\nAccept-Encoding: gzip, deflate\r\nProxy-Connection: Keep-Alive\r\nHost: www.codeproject.com\r\n");
    if (!getClient.SendHttpRequest())
    {
        return;
    }

    // 2. Post data to get the authentication cookie.
    WinHttpClient postClient(L"http://www.codeproject.com/script/Membership/LogOn.aspx?rp=%2fscript%2fMembership%2fLogOn.aspx");

    // Post data.
    // Change this to your Codeproject username and password.
    wstring username = L"YourCodeProjectUsername";
    wstring password = L"YourPassword";
    postClient.SetAdditionalRequestCookies(getClient.GetResponseCookies());
    string data = "FormName=MenuBarForm&Email=";
    data += (char *)_bstr_t(username.c_str());
    data += "&Password=";
    data += (char *)_bstr_t(password.c_str());
    data += "&RememberMeCheck=1";
    postClient.SetAdditionalDataToSend((BYTE *)data.c_str(), data.size());

    // Post headers.
    wstring headers = L"Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\nReferer: http://www.codeproject.com/script/Membership/LogOn.aspx\r\nAccept-Language: en-us\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; QQPinyin 730; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; CIBA; MS-RTC LM 8)\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: www.codeproject.com\r\nContent-Length: %d\r\nProxy-Connection: Keep-Alive\r\nPragma: no-cache\r\n";
    wchar_t szHeaders[MAX_PATH * 10] = L"";
    swprintf_s(szHeaders, MAX_PATH * 10, headers.c_str(), data.size());
    postClient.SetAdditionalRequestHeaders(szHeaders);
    if (!postClient.SendHttpRequest(L"POST", true))
    {
        return;
    }

    // 3. Finally get the zip file.    
    WinHttpClient downloadClient(L"http://www.codeproject.com/KB/IP/win_HTTP_wrapper/WinHttpClient_Src.zip");
    downloadClient.SetUserAgent(L"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; CIBA; MS-RTC LM 8)");

    // Sending this cookie make server believe you have already logged in.
    downloadClient.SetAdditionalRequestCookies(postClient.GetResponseCookies());
    if (!downloadClient.SendHttpRequest())
    {
        return;
    }
    downloadClient.SaveResponseToFile(L"C:\\WinHttpClient_Src.zip");
}

int _tmain(int /*argc*/, _TCHAR** /*argv*/)
{
    CompleteTest();
	return 0;
}

