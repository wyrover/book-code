#include <iostream>
#include <string>

using namespace std;

class Socket
{
public:
    Socket(const string& hostname) {}
};

class HttpRequest
{
public:
    HttpRequest(const string& hostname) :
        sock_(new Socket(hostname)) {}
    void send(string soapMsg)
    {
        sock_ << soapMsg;
    }
    ~HttpRequest()
    {
        delete sock_;
    }
private:
    Socket* sock_;
};

void sendMyData(string soapMsg, string host)
{
    HttpRequest req(host);
    req.send(soapMsg);
    // Nothing to do here, because when req goes out of scope
    // everything is cleaned up.
}

int main()
{
    string s = "xml";
    sendMyData(s, "www.oreilly.com");
}