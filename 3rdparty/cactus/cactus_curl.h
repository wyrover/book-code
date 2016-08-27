#ifndef __CACTUS_CACTUS_CURL_H__
#define __CACTUS_CACTUS_CURL_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <curl/curl.h>

namespace cactus
{
class CACTUS_API CHttpClient
{
public:
    CHttpClient();
    ~CHttpClient();

public:
    /* cURL APIs */
    bool isCurlInit();
    bool performGet(const string8& getUrl);
    bool performGetInternal(const string8& getUrl,
                            const string8& oAuthHttpHeader);
    bool performPost(const string8& postUrl, string8 dataStr = "");
    bool performDelete(const string8& deleteUrl);
    void getLastWebResponse(string8& outWebResp /* out */);
    void getLastCurlError(string8& outErrResp /* out */);
    bool performPostBin(const string8& postUrl, char *pData, ULONG uLen);
    bool perform_dg_post_bin(const string8& postUrl, char* pData, ULONG uLen);

    /* cURL proxy APIs */
    string8& getProxyServerIp();
    string8& getProxyServerPort();
    string8& getProxyUserName();
    string8& getProxyPassword();
    void setProxyServerIp(string8& proxyServerIp /* in */);
    void setProxyServerPort(string8& proxyServerPort /* in */);
    void setProxyUserName(string8& proxyUserName /* in */);
    void setProxyPassword(string8& proxyPassword /* in */);

private:
    void clearCurlCallbackBuffers();
    void prepareCurlProxy();
    void prepareCurlCallback();
    void prepareStandardParams();

    /* Internal cURL related methods */
    int saveLastWebResponse(char*& data, size_t size);

private:
    static int curlCallback(char* data, size_t size, size_t nmemb, CHttpClient* PHttpClientObj);

private:
    CURL*   curl_handle_;
    string8 callback_data_;
    char*   error_buffer_;

    /* cURL flags */
    bool curl_proxy_params_set_;
    bool curl_login_params_set_;
    bool curl_callback_params_set_;

    /* cURL proxy data */
    string8 proxy_server_ip_;
    string8 proxy_server_port_;
    string8 proxy_username_;
    string8 proxy_password_;

public:
	DWORD   last_curl_code;

};


void utilMakeCurlParams(string8& outStr, string8& inParam1, string8& inParam2);

} // namespace cactus

#endif // __CACTUS_CACTUS_CURL_H__
