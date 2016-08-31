#ifndef __CACTUS_CACTUS_REST_H__
#define __CACTUS_CACTUS_REST_H__

#include <cactus/cactus.h>
#include <curl/curl.h>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>

namespace cactus
{


class CACTUS_API RestClient
{
public:
    /**
     * public data definitions
     */
    typedef std::map<std::string, std::string> headermap;

    /** response struct for queries */
    typedef struct {
        int code;
        std::string body;
        headermap headers;
    } response;
    /** struct used for uploading data */
    typedef struct {
        const char* data;
        size_t length;
    } upload_object;

    /** public methods */
    // Auth
    static void clearAuth();
    static void setAuth(const std::string& user, const std::string& password);
    // HTTP GET
    static response get(const std::string& url);
    // HTTP POST
    static response post(const std::string& url, const std::string& ctype,
                         const std::string& data);
    // HTTP PUT
    static response put(const std::string& url, const std::string& ctype,
                        const std::string& data);
    // HTTP DELETE
    static response del(const std::string& url);

private:
    // writedata callback function
    static size_t write_callback(void *ptr, size_t size, size_t nmemb,
                                 void *userdata);

    // header callback function
    static size_t header_callback(void *ptr, size_t size, size_t nmemb,
                                  void *userdata);
    // read callback function
    static size_t read_callback(void *ptr, size_t size, size_t nmemb,
                                void *userdata);
    static const char* user_agent;
    static std::string user_pass;
};

} // namespace cactus

#endif // __CACTUS_CACTUS_REST_H__
