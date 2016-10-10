#include <windows.h>
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>


struct Message {
    std::string message;
    std::string* result;
    //Session* self;
    //std::vector<Session*>* session_pool;
};


class HandlerBase
{
public:
    HandlerBase();
    HandlerBase(const HandlerBase& orig);
    virtual ~HandlerBase();
public:
    virtual void run(Message* msg);
};

HandlerBase::HandlerBase()
{
}

HandlerBase::HandlerBase(const HandlerBase& orig)
{
}

HandlerBase::~HandlerBase()
{
}

void HandlerBase::run(Message* msg)
{
    std::cout << "[Handler: GET] Input: " << msg->message << std::endl;
}

////////////////////////////////////////
class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();
public:
    void registerHandler(std::string name, HandlerBase* handler);
    void handle(Message* msg);
private:
    std::map<std::string, class HandlerBase*> handlers;
};

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
    for (std::map<std::string, HandlerBase*>::iterator it = this->handlers.begin(); it != this->handlers.end(); ++it) {
        delete it->second;
    }
}

void Dispatcher::registerHandler(std::string name, HandlerBase* handler)
{
    /**Convert name to uppercase*/
    std::locale locale;

    for (std::string::size_type i = 0; i < name.length(); ++i)
        name[i] = std::toupper(name[i], locale);

    /**conversion end*/

    if (handlers.find(name) == handlers.end()) {
        std::cout << "[System] Registrering handler: " << name << " ... done!" << std::endl;
        handlers.insert(std::pair<std::string, HandlerBase*>(name, handler));
    } else {
        //deletes handler in case if handler is not added to map
        //Garbage collection.
        delete handler;
        std::cout << "[System] Handler with name " << name << " Is already registered." << std::endl;
    }
}

void Dispatcher::handle(Message *msg)
{
    //Remove newline characters
    for (int i = 0; i < msg->message.length(); i++) {
        if (msg->message[i] == '\n') {
            msg->message[i] = ' ';
        }
    }

    std::string body;
    std::string action;
    //parseMessage(msg->message, action, body);
    msg->message = body;

    if (handlers.find(action) != handlers.end()) {
        handlers.find(action)->second->run(msg);
    } else {
        *msg->result = "ERROR\n";
    }
}




int main()
{
    SetConsoleOutputCP(65001);
    Dispatcher dispatcher;
    dispatcher.registerHandler("GET", new HandlerBase());
//    boost::asio::io_service io_service;
//    Server s(io_service, atoi(argv[1]), &dispatcher);
//    io_service.run();
    system("pause");
    return 0;
}