#include <windows.h>
#include <iostream>
#include <cstdio>
#include <map>

///////////////////////////////////////
class BaseController
{
protected:
    BaseController();
    ~BaseController();

public:
    virtual void onMsgResponse(const unsigned int opcode, const char* msg, const unsigned int size) = 0;
};


BaseController::BaseController()
{
}

BaseController::~BaseController()
{
}

//////////////////////////////////////

class MessageDispatcher
{
public:
    static MessageDispatcher* getInstance();

    void registController(unsigned int moduleno, BaseController * controller);
    void dispatchMessage(const unsigned int opcode, const char * buf, const unsigned short size);

private:
    MessageDispatcher();

    static MessageDispatcher* dispatcher_;

    std::map<unsigned int, BaseController*> controllers_;
};

///////////////////////////////////

MessageDispatcher* MessageDispatcher::dispatcher_ = nullptr;

MessageDispatcher* MessageDispatcher::getInstance()
{
    if (dispatcher_ == nullptr) {
        dispatcher_ = new MessageDispatcher();
    }

    return dispatcher_;
}

MessageDispatcher::MessageDispatcher()
{
    controllers_.clear();
}

void MessageDispatcher::registController(unsigned int moduleno, BaseController * controller)
{
    std::pair<std::map<unsigned int, BaseController*>::iterator, bool> result;
    result = controllers_.insert(std::pair<unsigned int, BaseController*>(moduleno, controller));

    if (result.second == false) {
        fprintf(stderr, "register module failed.");
    } else {
        printf("register module success.");
    }
}

void MessageDispatcher::dispatchMessage(const unsigned int opcode, const char * buf, const unsigned short size)
{
    unsigned int moduleno = opcode >> 16;
    unsigned int msgno = opcode & 0x0000FFFF;

    if (moduleno == 0) {
        return;
    }

    std::map<unsigned int, BaseController*>::iterator iter = controllers_.find(moduleno);

    if (iter != controllers_.end()) {
        BaseController* pController = iter->second;

        if (pController != nullptr) {
            pController->onMsgResponse(opcode, buf, size);
        }
    } else {
        fprintf(stderr, "illegal message.");
    }
}





///////////////////////////////////////
int main()
{
    SetConsoleOutputCP(65001);
    std::cout << "中文测试" << std::endl;
    system("pause");
    return 0;
}