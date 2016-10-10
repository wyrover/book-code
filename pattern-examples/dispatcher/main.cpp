#include <map>

class Message;

class MessageListener
{
public:
    MessageListener();
    ~MessageListener();
public:
    virtual void Recive
    Message(Message* message) = 0;
}

class MessageDispatcher
{
    typedef std::multimap<const char*, MessageListener*> MsgListenerMultiMap;
    typedef MsgListenerMultiMap::iterator MsgListenerMultiMapIter;

public:
    MessageDispatcher();
    ~MessageDispatcher();

public:
    void RegisterListener(const char* subject, MessageListener* listener);

    template<typename MessageT>
    MessageT* NewMessage();

    void DispatchMessage(Message* message);

private:
    MsgListenerMultiMap listeners_;
    BLockAllocator* message_allocator_;
}

template <typename MessageT>
MessageT* MessageDispatcher::NewMessage()
{
    // m_message_allocator.Allocate(message_size);
    // Allocate a MessageT message into the m_message_allocator.
    // Return a pointer to that message to the client.
    // Note: The allocated message will be deallocated when DispatchMessage is
    // performed.
    //return m_message_allocator.Allocate(sizeof(MessageT));
    return nullptr; // TMP
}




MessageDispatcher::MessageDispatcher()
{
}

MessageDispatcher::~MessageDispatcher()
{
}

void
MessageDispatcher::RegisterListener(const char *subject, MessageListener* listener)
{
    listeners_.insert(std::pair<const char*, MessageListener*>(subject, listener));
}

void
MessageDispatcher::DispatchMessage(Message *message)
{
    std::pair<MsgListenerMultiMapIter, MsgListenerMultiMapIter> listener_range = listeners_.equal_range(message->GetSubject());
    MsgListenerMultiMapIter it;

    for (it = listener_range.first;  it != listener_range.second;  ++it) {
        (*it).second->ReciveMessage(message);
    }

//    Deallocate message from the m_message_allocator
}