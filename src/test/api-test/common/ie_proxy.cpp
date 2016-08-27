#include "ie_proxy.h"

namespace cactus
{

std::wstring CIEProxy::GetIEProxy(const std::wstring& strURL, const E_proxy_type& eProxyType)
{
    std::wstring strRet_cswuyg;
    WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions = {0};
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieProxyConfig = {0};
    BOOL bAutoDetect = FALSE; //“自动检测设置”，但有时候即便选择上也会返回0，所以需要根据url判断

    if (::WinHttpGetIEProxyConfigForCurrentUser(&ieProxyConfig)) {
        if (ieProxyConfig.fAutoDetect) {
            bAutoDetect = TRUE;
        }

        if (ieProxyConfig.lpszAutoConfigUrl != NULL) {
            bAutoDetect = TRUE;
            autoProxyOptions.lpszAutoConfigUrl = ieProxyConfig.lpszAutoConfigUrl;
        }
    } else {
        // error
        return strRet_cswuyg;
    }

    if (bAutoDetect) {
        if (autoProxyOptions.lpszAutoConfigUrl != NULL) {
            autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
        } else {
            autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
            autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
        }

        autoProxyOptions.fAutoLogonIfChallenged = TRUE;
        HINTERNET hSession = ::WinHttpOpen(0, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                           WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC);

        if (hSession != NULL) {
            WINHTTP_PROXY_INFO autoProxyInfo = {0};
            bAutoDetect = ::WinHttpGetProxyForUrl(hSession, strURL.c_str(), &autoProxyOptions, &autoProxyInfo);

            if (hSession != NULL) {
                ::WinHttpCloseHandle(hSession);
            }

            if (autoProxyInfo.lpszProxy) {
                if (autoProxyInfo.lpszProxyBypass == NULL || CheckByPass(strURL, autoProxyInfo.lpszProxyBypass)) {
                    std::wstring strProxyAddr = autoProxyInfo.lpszProxy;
                    strRet_cswuyg = GetProxyFromString(eProxyType, strProxyAddr);
                }

                if (autoProxyInfo.lpszProxy != NULL) {
                    ::GlobalFree(autoProxyInfo.lpszProxy);
                }

                if (autoProxyInfo.lpszProxyBypass != NULL) {
                    ::GlobalFree(autoProxyInfo.lpszProxyBypass);
                }
            }
        }
    }

    //when strRet is empty, even though the user has set proxy auto-config, we need to check the manual setting.
    if (strRet_cswuyg.empty()) {
        if (ieProxyConfig.lpszProxy != NULL) {
            if (ieProxyConfig.lpszProxyBypass == NULL || CheckByPass(strURL, ieProxyConfig.lpszProxyBypass)) {
                std::wstring strProxyAddr = ieProxyConfig.lpszProxy;
                strRet_cswuyg = GetProxyFromString(eProxyType, strProxyAddr);
            }
        }
    }

    if (ieProxyConfig.lpszAutoConfigUrl != NULL) {
        ::GlobalFree(ieProxyConfig.lpszAutoConfigUrl);
    }

    if (ieProxyConfig.lpszProxy != NULL) {
        ::GlobalFree(ieProxyConfig.lpszProxy);
    }

    if (ieProxyConfig.lpszProxyBypass != NULL) {
        ::GlobalFree(ieProxyConfig.lpszProxyBypass);
    }

    return strRet_cswuyg;
}

BOOL CIEProxy::CheckByPass(const std::wstring& strURL, const std::wstring& strPassBy)
{
    BOOL bRet = TRUE;
    std::wstring strPassTemp = strPassBy;

    while (!strPassTemp.empty()) {
        std::wstring strPrePart = strPassTemp;
        size_t pos = strPassTemp.find(L";");

        if (pos != std::wstring::npos) {
            strPrePart = strPassTemp.substr(0, pos);
            strPassTemp = strPassTemp.substr(pos + 1);
        } else {
            strPrePart = strPassTemp;
            strPassTemp = L"";
        }

        if (strPrePart == L"<local>") {
            if (strURL.find(L".") == std::wstring::npos) {
                bRet = FALSE;
                break;
            } else if (strURL.find(L"127.0.0.1") != std::wstring::npos) {
                bRet = FALSE;
                break;
            }
        } else if (strURL.find(strPrePart) != std::wstring::npos) {
            bRet = FALSE;
            break;
        }
    }

    return bRet;
}

std::wstring CIEProxy::GetProxyFromString(const E_proxy_type& eProxyType, const std::wstring& strProxys)
{
    ///strProxys may be like this: "http=127.0.0.1:8888;https=127.0.0.1:8888;ftp=127.0.0.1:8888;socks=127.0.0.1:8888"  or "127.0.0.1:8888"
    std::wstring strRet = strProxys;

    if (strProxys.find(L'=') != std::wstring::npos) {
        std::wstring strType;

        if (eProxyType == eProxyType_http) {
            strType = L"http=";
        } else if (eProxyType == eProxyType_ftp) {
            strType = L"ftp=";
        } else if (eProxyType == eProxyType_https) {
            strType = L"https=";
        } else if (eProxyType == eProxyType_socks) {
            strType = L"socks=";
        }

        if (!strType.empty()) {
            std::wstring::size_type nStart = strProxys.find(strType);

            if (nStart != std::wstring::npos) {
                nStart += strType.length();
                int nLen = 0;
                std::wstring::size_type nNext = strProxys.find(L';', nStart);

                if (nNext != std::wstring::npos) {
                    nLen = nNext - nStart;
                } else {
                    nLen = strProxys.length() - nStart;
                }

                strRet = strProxys.substr(nStart, nLen);
            } else {
                strRet = L"";
            }
        }
    }

    return strRet;
}

}