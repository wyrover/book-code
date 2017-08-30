#include <cactus/cactus_curl.h>

#ifdef _DEBUG
    #pragma comment(lib, "libcurl_debug.lib")
#else
    #pragma comment(lib, "libcurl.lib")
#endif


namespace cactus
{



CHttpClient::CHttpClient():
    curl_handle_(NULL),
    curl_proxy_params_set_(false),
    curl_login_params_set_(false),
    curl_callback_params_set_(false),
	last_curl_code(0)
{
    error_buffer_ = (char*)malloc(1024);
    clearCurlCallbackBuffers();
    curl_handle_ = curl_easy_init();

    if (NULL == curl_handle_) {
        string8 dummyStr;
        getLastCurlError(dummyStr);
    }

    curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYPEER, 0);
}



CHttpClient::~CHttpClient()
{
    if (curl_handle_) {
        curl_easy_cleanup(curl_handle_);
        curl_handle_ = NULL;
    }

    if (error_buffer_) {
        free(error_buffer_);
        error_buffer_ = NULL;
    }
}

void CHttpClient::clearCurlCallbackBuffers()
{
    callback_data_ = "";
    memset(error_buffer_, 0, 1024);
}

void CHttpClient::prepareCurlProxy()
{
    if (curl_proxy_params_set_) {
        return;
    }

    /* Reset existing proxy details in cURL */
    curl_easy_setopt(curl_handle_, CURLOPT_PROXY, NULL);
    curl_easy_setopt(curl_handle_, CURLOPT_PROXYUSERPWD, NULL);
    curl_easy_setopt(curl_handle_, CURLOPT_PROXYAUTH, (long)CURLAUTH_ANY);
    /* Set proxy details in cURL */
    string8 proxyIpPort("");

    if (getProxyServerIp().size()) {
        utilMakeCurlParams(proxyIpPort, getProxyServerIp(), getProxyServerPort());
    }

    curl_easy_setopt(curl_handle_, CURLOPT_PROXY, proxyIpPort.c_str());

    /* Prepare username and password for proxy server */
    if (proxy_username_.length() && proxy_password_.length()) {
        string8 proxyUserPass;
        utilMakeCurlParams(proxyUserPass, getProxyUserName(), getProxyPassword());
        curl_easy_setopt(curl_handle_, CURLOPT_PROXYUSERPWD, proxyUserPass.c_str());
    }

    /* Set the flag to true indicating that proxy info is set in cURL */
    curl_proxy_params_set_ = true;
}

void CHttpClient::prepareCurlCallback()
{
    if (curl_callback_params_set_) {
        return;
    }

    /* Set buffer to get error */
    curl_easy_setopt(curl_handle_, CURLOPT_ERRORBUFFER, error_buffer_);
    /* Set callback function to get response */
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, curlCallback);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, this);
    /* Set the flag to true indicating that callback info is set in cURL */
    curl_callback_params_set_ = true;
}



void CHttpClient::prepareStandardParams()
{
    curl_easy_setopt(curl_handle_, CURLOPT_CUSTOMREQUEST, NULL);
    curl_easy_setopt(curl_handle_, CURLOPT_ENCODING, "");
    clearCurlCallbackBuffers();
    prepareCurlProxy();
    prepareCurlCallback();
}

bool CHttpClient::performGet(const string8& getUrl)
{
    /* Return if cURL is not initialized */
    if (!isCurlInit()) {
        return false;
    }

    string8 dataStrDummy;
    string8 oAuthHttpHeader;
    /* Prepare standard params */
    prepareStandardParams();
    /* Set http request and url */
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, 60);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 90); 
    curl_easy_setopt(curl_handle_, CURLOPT_URL, getUrl.c_str());

    /* Send http request */
	last_curl_code = curl_easy_perform(curl_handle_); 
    if (CURLE_OK == last_curl_code) {
        return true;
    }

    return false;
}

bool CHttpClient::performGetInternal(const string8& getUrl, const string8& oAuthHttpHeader)
{
    if (!isCurlInit()) {
        return false;
    }

    struct curl_slist* pOAuthHeaderList = NULL;

    /* Prepare standard params */
    prepareStandardParams();

    /* Set http request and url */
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPGET, 1);

    curl_easy_setopt(curl_handle_, CURLOPT_URL, getUrl.c_str());

    /* Set header */
    if (oAuthHttpHeader.length()) {
        pOAuthHeaderList = curl_slist_append(pOAuthHeaderList, oAuthHttpHeader.c_str());

        if (pOAuthHeaderList) {
            curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, pOAuthHeaderList);
        }
    }

    /* Send http request */
    if (CURLE_OK == curl_easy_perform(curl_handle_)) {
        if (pOAuthHeaderList) {
            curl_slist_free_all(pOAuthHeaderList);
        }

        return true;
    }

    if (pOAuthHeaderList) {
        curl_slist_free_all(pOAuthHeaderList);
    }

    return false;
}

bool CHttpClient::performDelete(const string8& deleteUrl)
{
    if (!isCurlInit()) {
        return false;
    }

    string8 dataStrDummy;
    string8 oAuthHttpHeader;
    /* Prepare standard params */
    prepareStandardParams();
    /* Set http request and url */
    curl_easy_setopt(curl_handle_, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl_handle_, CURLOPT_URL, deleteUrl.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_COPYPOSTFIELDS, dataStrDummy.c_str());

    /* Send http request */
    if (CURLE_OK == curl_easy_perform(curl_handle_)) {
        return true;
    }

    return false;
}

bool CHttpClient::performPost(const string8& postUrl, string8 dataStr /*= ""*/)
{
    if (!isCurlInit()) {
        return false;
    }

    string8 oAuthHttpHeader;
    prepareStandardParams();
    /* Set http request, url and data */
    curl_easy_setopt(curl_handle_, CURLOPT_POST, 1);
    curl_easy_setopt(curl_handle_, CURLOPT_URL, postUrl.c_str());

    if (dataStr.length()) {
        curl_easy_setopt(curl_handle_, CURLOPT_COPYPOSTFIELDS, dataStr.c_str());
    }

    /* Send http request */
    if (CURLE_OK == curl_easy_perform(curl_handle_)) {
        return true;
    }

    return false;
}


bool CHttpClient::performPostBin(const string8& postUrl, char *pData, ULONG uLen)
{
	if (!isCurlInit()) {
		return false;
	}

	if(uLen <= 0)
		return false;

	string8 oAuthHttpHeader;
	prepareStandardParams();
	/* Set http request, url and data */
	curl_easy_setopt(curl_handle_, CURLOPT_POST, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_URL, postUrl.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_COPYPOSTFIELDS, pData);
	curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE,  uLen);

	/* Send http request */
	if (CURLE_OK == curl_easy_perform(curl_handle_)) {
		return true;
	}

	return false;
}

bool CHttpClient::perform_dg_post_bin(const string8& postUrl, char* pData, ULONG uLen)
{
    if (!isCurlInit()) {
        return false;
    }

    if(uLen <= 0)
        return false;

    int timeout = 30;

    prepareStandardParams();
    /* Set http request, url and data */
    /* Set header */
    struct curl_slist* http_headers = NULL;
    http_headers = curl_slist_append(http_headers, "Content-Type: multipart/form-data");
    if (http_headers) {
        curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(curl_handle_, CURLOPT_POST, 1);
        curl_easy_setopt(curl_handle_, CURLOPT_URL, postUrl.c_str());
        curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT, timeout); 
        curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 60); 
        curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, pData);
        curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE,  uLen + 1);

        /* Send http request */
		last_curl_code = curl_easy_perform(curl_handle_); 
        if (CURLE_OK == last_curl_code) {
            if (http_headers) {
                curl_slist_free_all(http_headers);
            }
            return true;
        }
    }

    if (http_headers) {
        curl_slist_free_all(http_headers);
    }

    return false;
}

int CHttpClient::curlCallback(char* data, size_t size, size_t nmemb, CHttpClient* PHttpClientObj)
{
    if (PHttpClientObj && data) {
        return PHttpClientObj->saveLastWebResponse(data, (size * nmemb));
    }

    return 0;
}

bool CHttpClient::isCurlInit()
{
    return (NULL != curl_handle_) ? true : false;
}

void CHttpClient::getLastWebResponse(string8& outWebResp /* out */)
{
    outWebResp = "";

    if (callback_data_.length()) {
        outWebResp = callback_data_;
    }
}

void CHttpClient::getLastCurlError(string8& outErrResp /* out */)
{
    error_buffer_[1024 - 1] = '\0';
    outErrResp.assign(error_buffer_);
}

int CHttpClient::saveLastWebResponse(char*& data, size_t size)
{
    if (data && size) {
        /* Append data in our internal buffer */
        callback_data_.append(data, size);
        return (int)size;
    }

    return 0;
}

string8& CHttpClient::getProxyServerIp()
{
    return proxy_server_ip_;
}

string8& CHttpClient::getProxyServerPort()
{
    return proxy_server_port_;
}

string8& CHttpClient::getProxyUserName()
{
    return proxy_username_;
}

string8& CHttpClient::getProxyPassword()
{
    return proxy_password_;
}

void CHttpClient::setProxyServerIp(string8& proxyServerIp /* in */)
{
    if (proxyServerIp.length()) {
        proxy_server_ip_ = proxyServerIp;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
        curl_proxy_params_set_ = false;
    }
}

void CHttpClient::setProxyServerPort(string8& proxyServerPort /* in */)
{
    if (proxyServerPort.length()) {
        proxy_server_port_ = proxyServerPort;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
        curl_proxy_params_set_ = false;
    }
}

void CHttpClient::setProxyUserName(string8& proxyUserName /* in */)
{
    if (proxyUserName.length()) {
        proxy_username_ = proxyUserName;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
        curl_proxy_params_set_ = false;
    }
}

void CHttpClient::setProxyPassword(string8& proxyPassword /* in */)
{
    if (proxyPassword.length()) {
        proxy_password_ = proxyPassword;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
        curl_proxy_params_set_ = false;
    }
}



void utilMakeCurlParams(string8& outStr, string8& inParam1, string8& inParam2)
{
    outStr = inParam1;
    outStr += ":" + inParam2;
}

} // namespace cactus