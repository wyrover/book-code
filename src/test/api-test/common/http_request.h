#ifndef http_request_h__
#define http_request_h__

#include<WS2tcpip.h>
#include <iostream>

class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();
    void printIP();
    void setUrl(PCSTR url);
    void setPort(PCSTR port);
    void addHeader(char *str);
    void addHeaderln(char *str);
    void setFamily(int fm);
    void setBuffer(int size);
    void setTservname(PCSTR serverName);
    void setIpPortocol(int ipProto);
    void setSocketType(int type);
    /*
        0 : success
        -1: socket error
        1 : url = null
        2 : Failed to set target Address.
        3 : Socket Failed!
        else
            connect function errcode
    */
    int scand();
    int scandAndSaveToTxt();
private:
    int bufferSize;
    HttpRequest & operator=(HttpRequest & tmp);
    PCSTR url;
    PCSTR port;
    char *buffer;
    std::string header;
    addrinfo hint;
    addrinfo *targetAddressInfo;
    SOCKET webSocket;
    WSADATA wsaData;
};

#endif // http_request_h__
