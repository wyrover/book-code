#include "http_request.h"

HttpRequest::HttpRequest() : bufferSize(10000), url(NULL), port(NULL), buffer(NULL)
{
    while (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed.\n" << std::endl;
    }

    ZeroMemory(&hint, sizeof(hint));
    webSocket           = INVALID_SOCKET;
    hint.ai_family      = AF_INET;
    hint.ai_protocol    = IPPROTO_TCP;
    hint.ai_socktype    = SOCK_STREAM;
    hint.ai_flags       = 0;
    targetAddressInfo   = NULL;
}

HttpRequest::~HttpRequest()
{
    freeaddrinfo(targetAddressInfo);
    closesocket(webSocket);
    WSACleanup();
}

void HttpRequest::printIP()
{
    DWORD getAddressRes = getaddrinfo(url, port, &hint, &targetAddressInfo);

    for (addrinfo *i = targetAddressInfo; i != NULL; i = i->ai_next) {
        sockaddr_in &address = *(sockaddr_in*)(i->ai_addr);
        std::cout << static_cast<int>(address.sin_addr.S_un.S_un_b.s_b1) << '.'
                  << static_cast<int>(address.sin_addr.S_un.S_un_b.s_b2) << '.'
                  << static_cast<int>(address.sin_addr.S_un.S_un_b.s_b3) << '.'
                  << static_cast<int>(address.sin_addr.S_un.S_un_b.s_b4) << "::"
                  << address.sin_port << std::endl;
    }
}

void HttpRequest::setUrl(PCSTR url)
{
    this->url = url;
}

void HttpRequest::setPort(PCSTR port)
{
    this->port = port;
}

void HttpRequest::addHeader(char *str)
{
    header += str;
}

void HttpRequest::addHeaderln(char * str)
{
    header += str;
    header += "\r\n";
}

void HttpRequest::setFamily(int fm)
{
    hint.ai_family = fm;
}

void HttpRequest::setBuffer(int size)
{
    bufferSize = size;
}

void HttpRequest::setTservname(PCSTR serverName)
{
    port = serverName;
}

void HttpRequest::setIpPortocol(int ipProto)
{
    hint.ai_protocol = ipProto;
}

void HttpRequest::setSocketType(int type)
{
    hint.ai_socktype = type;
}

int HttpRequest::scand()
{
    if (url == NULL)
        return 1;

    DWORD getAddressRes = getaddrinfo(url, port, &hint, &targetAddressInfo);

    if (getAddressRes != 0 || targetAddressInfo == NULL)
        return 2;

    webSocket = socket(hint.ai_family, hint.ai_socktype, hint.ai_protocol);

    if (webSocket == INVALID_SOCKET)
        return 3;

    for (addrinfo *i = targetAddressInfo; i != NULL; i = i->ai_next) {
        SOCKADDR *address = (SOCKADDR*)i->ai_addr;
        int errcode = connect(webSocket, address, sizeof(*address));

        if (errcode != 0)
            continue;

        int sentBytes = send(webSocket, header.c_str(), header.length(), hint.ai_flags);

        if (sentBytes < header.length() || sentBytes == SOCKET_ERROR)
            continue;

        bool index = false;
        buffer = new char[bufferSize];

        while (recv(webSocket, buffer, bufferSize, 0) > 0) {
            index = true;

            for (int index = 0; buffer[index] > 0; ++index) {
                std::cout << buffer[index];
            }
        }

        if (index)
            break;
    }

    return 0;
}

int HttpRequest::scandAndSaveToTxt()
{
    std::string URL(url);

    //FileManager fp(URL += ".txt");
    if (url == NULL)
        return 1;

    DWORD getAddressRes = getaddrinfo(url, port, &hint, &targetAddressInfo);

    if (getAddressRes != 0 || targetAddressInfo == NULL)
        return 2;

    webSocket = socket(hint.ai_family, hint.ai_socktype, hint.ai_protocol);

    if (webSocket == INVALID_SOCKET)
        return 3;

    for (addrinfo *i = targetAddressInfo; i != NULL; i = i->ai_next) {
        SOCKADDR *address = (SOCKADDR*)i->ai_addr;
        int errcode = connect(webSocket, address, sizeof(*address));

        if (errcode != 0)
            continue;

        int sentBytes = send(webSocket, header.c_str(), header.length(), hint.ai_flags);

        if (sentBytes < header.length() || sentBytes == SOCKET_ERROR)
            continue;

        bool index = false;
        buffer = new char[bufferSize];

        while (recv(webSocket, buffer, bufferSize, 0) > 0) {
            index = true;

            //fp.saveString(buffer);
            for (int index = 0; buffer[index] > 0; ++index) {
                std::cout << buffer[index];
            }
        }

        if (index)
            break;
    }

    return 0;
}