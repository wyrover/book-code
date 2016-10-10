#include <windows.h>
#include <string>
#include <memory>
#include <iostream>
#include <map>

class Dispatcher;

class MessageReceiver
{
public:
    typedef std::shared_ptr<MessageReceiver> MessRecPtr;
    MessageReceiver(int id, Dispatcher* disp);
    int GetID() const;
    Dispatcher* GetDisp();
    virtual void ReceiveMessage(const std::string& message, int sender = 0) = 0;

protected:
    void SendBack(int sender, const std::string& message);

private:
    int m_id;
    Dispatcher* m_disp;
};

////////////////////////////



////////////////////////////


class Reciever : public MessageReceiver
{
public:
    Reciever(int id, Dispatcher* disp)
        : MessageReceiver(id, disp)
    {
    }

    void ReceiveMessage(const std::string& message, int sender = 0)
    {
        std::cout << "I, " << this->GetID() << ", got a message: " << message << "\n";
    }
};


class RepliedReceiver : public MessageReceiver
{
public:
    RepliedReceiver(int id, Dispatcher* disp)
        : MessageReceiver(id, disp)
    {
    }

    void ReceiveMessage(const std::string& message, int sender)
    {
        std::cout << "I, " << this->GetID() << ", got a message: " << message << "\n";
        SendBack(sender, message);
    }
};


class Dispatcher
{
public:
    ~Dispatcher()
    {
    };

    static Dispatcher* GetInstance();
    void SendMessage(int sender, int receiver, const std::string& message);
    void AddMessageReceiver(MessageReceiver::MessRecPtr rec);
    void RemoveMessageReceiver(MessageReceiver::MessRecPtr rec);

private:
    Dispatcher() {};
    static Dispatcher* m_instance;
    std::map<int, MessageReceiver::MessRecPtr> subscribers;
};

////////////////////////////

Dispatcher* Dispatcher::m_instance = nullptr;

Dispatcher* Dispatcher::GetInstance()
{
    if (m_instance == nullptr) {
        m_instance = new Dispatcher;
    }

    return m_instance;
}

void Dispatcher::SendMessage(int sender, int receiver, const std::string& message)
{
    subscribers.find(receiver)->second->ReceiveMessage(message, sender);
}

void Dispatcher::AddMessageReceiver(MessageReceiver::MessRecPtr rec)
{
    subscribers.insert(std::pair<int, MessageReceiver::MessRecPtr>(rec->GetID(), rec));
}

void Dispatcher::RemoveMessageReceiver(MessageReceiver::MessRecPtr rec)
{
    subscribers.erase(rec->GetID());
}

////////////////////////////


MessageReceiver::MessageReceiver(int id, Dispatcher* disp)
    : m_id(id)
    , m_disp(disp)
{
}

int MessageReceiver::GetID() const
{
    return m_id;
}

Dispatcher* MessageReceiver::GetDisp()
{
    return m_disp;
}

void MessageReceiver::SendBack(int sender, const std::string& message)
{
    m_disp->SendMessage(m_id, sender, message);
}

///////



typedef std::shared_ptr<Reciever> RecieverPtr;
typedef std::shared_ptr<RepliedReceiver> RepliedReceiverPtr;

int main()
{
    SetConsoleOutputCP(65001);
    Dispatcher::GetInstance()->AddMessageReceiver(std::make_shared<Reciever>(1, Dispatcher::GetInstance()));
    Dispatcher::GetInstance()->AddMessageReceiver(std::make_shared<Reciever>(2, Dispatcher::GetInstance()));
    Dispatcher::GetInstance()->AddMessageReceiver(std::make_shared<RepliedReceiver>(3, Dispatcher::GetInstance()));
    Dispatcher::GetInstance()->SendMessage(2, 1, "hello from 2");
    Dispatcher::GetInstance()->SendMessage(3, 2, "hello from 3");
    Dispatcher::GetInstance()->SendMessage(1, 3, "hello from 1");
    system("pause");
    return 0;
}