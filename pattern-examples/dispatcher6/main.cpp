#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <map>

using namespace std;


#define EVENT_TYPE unsigned char
#define EVENT_SUB_TYPE unsigned short
#define EVENT_TYPE_LENGTH sizeof(EVENT_TYPE)
#define EVENT_SUB_TYPE_LENGTH sizeof(EVENT_SUB_TYPE)


enum EventType : EVENT_TYPE {
    UNDEF_EVENT = 0,
    NETWORK_EVENT,
    STATE_EVENT,
    OBJECT_EVENT,
    SDL_INPUT_EVENT,
    SDL_WINDOW_EVENT,

    EVENT_TYPE_COUNT
};


enum EventSubType : EVENT_SUB_TYPE {
    UNDEF_SUB_EVENT = 0,

    // Network events
    NETWORK_JOIN,
    NETWORK_PART,
    NETWORK_DATA_IN,
    NETWORK_PING,
    NETWORK_PONG,

    // State events
    STATE_RUN_START,
    STATE_RUN_PAUSE ,

    // Object events
    OBJECT_CREATE,
    OBJECT_DESTROY,
    OBJECT_UPDATE,
    OBJECT_PARENT_ADD,
    OBJECT_PARENT_REMOVE,
    OBJECT_CHILD_ADD,
    OBJECT_CHILD_REMOVE,

    // SDL input events
    SDL_MOUSE_DOWN,
    SDL_MOUSE_UP,
    SDL_MOUSE_MOVE,
    SDL_MOUSE_WHEEL,
    SDL_KEY_DOWN,
    SDL_KEY_UP,
    SDL_TEXT_INPUT,
    SDL_TEXT_EDITING,
    SDL_JOYSTICK_INPUT,

    // SDL Window events
    SDL_WINDOW_RESIZE,
    SDL_WINDOW_FOCUS_CHANGE,

    EVENT_SUB_TYPE_COUNT
};



struct Event {
    Event();

    EventType      type;
    EventSubType   subType;
    //ExecutionTimer timer;
};


std::string EventTypeToStr(EventType);
std::string EventSubTypeToStr(EventSubType);


struct EventListener {
    virtual void HandleEvent(Event*) = 0;
};


typedef std::shared_ptr<EventListener> EventListenerPtr;


struct EventListenerCollection {
    std::mutex collectionMutex;
    std::vector<EventListenerPtr> collection;
};


// 单事件，多处理器
class EventDispatcher
{
public:
    EventDispatcher();
    ~EventDispatcher();

    void HandleEvent(Event*);

    void AddEventListener(EventType, EventListenerPtr);

    std::mutex eventListenersMutex;
    std::map<EventType, EventListenerCollection*> eventListeners;
};



class EventQueue
{
public:
    ~EventQueue();

    Event* GetEvent();
    void   AddEvent(Event*);

    size_t GetEventCount();


private:
    std::mutex          eventQueueMutex;
    std::vector<Event*> eventQueue;
};



////////////////////////////////////////////////////////////////
// 实现文件
//

Event::Event() : type(UNDEF_EVENT),
    subType(UNDEF_SUB_EVENT)
{
    //timer.Reset();
}



string EventTypeToStr(EventType type)
{
    string ret;

    switch (type) {
    case UNDEF_EVENT:
        ret = "Undefined Event";
        break;

    case NETWORK_EVENT:
        ret = "Network Event";
        break;

    case STATE_EVENT:
        ret = "State Event";
        break;

    case OBJECT_EVENT:
        ret = "Object Event";
        break;

    case SDL_INPUT_EVENT:
        ret = "SDL Input Event";
        break;

    case SDL_WINDOW_EVENT:
        ret = "SDL Window Event";
        break;

    default:
        ret = "Unknown Event";
    }

    return ret;
}



string EventSubTypeToStr(EventSubType type)
{
    string ret;

    switch (type) {
    case UNDEF_SUB_EVENT:
        ret = "Undefined Sub Event";
        break;

    case NETWORK_JOIN:
        ret = "Network Join";
        break;

    case NETWORK_PART:
        ret = "Network Part";
        break;

    case NETWORK_DATA_IN:
        ret = "Network Data In";
        break;

    case NETWORK_PING:
        ret = "Network Ping";
        break;

    case NETWORK_PONG:
        ret = "Network Pong";
        break;

    case STATE_RUN_START:
        ret = "State Run Start";
        break;

    case STATE_RUN_PAUSE:
        ret = "State Run Pause";
        break;

    case OBJECT_CREATE:
        ret = "Object Create";
        break;

    case OBJECT_DESTROY:
        ret = "Object Destroy";
        break;

    case OBJECT_UPDATE:
        ret = "Object Update";
        break;

    case OBJECT_PARENT_ADD:
        ret = "Object Parent Add";
        break;

    case OBJECT_PARENT_REMOVE:
        ret = "Object Parent Remove";
        break;

    case OBJECT_CHILD_ADD:
        ret = "Object Child Add";
        break;

    case OBJECT_CHILD_REMOVE:
        ret = "Object Child Remove";
        break;

    case SDL_MOUSE_DOWN:
        ret = "SDL Mouse Down";
        break;

    case SDL_MOUSE_UP:
        ret = "SDL Mouse Up";
        break;

    case SDL_MOUSE_MOVE:
        ret = "SDL Mouse Move";
        break;

    case SDL_MOUSE_WHEEL:
        ret = "SDL Mouse Wheel";
        break;

    case SDL_KEY_DOWN:
        ret = "SDL Key Down";
        break;

    case SDL_KEY_UP:
        ret = "SDL Key Up";
        break;

    case SDL_TEXT_INPUT:
        ret = "SDL Text Input";
        break;

    case SDL_TEXT_EDITING:
        ret = "SDL Text Editing";
        break;

    case SDL_JOYSTICK_INPUT:
        ret = "SDL Joystick Input";
        break;

    case SDL_WINDOW_RESIZE:
        ret = "SDL Window Resize";
        break;

    case SDL_WINDOW_FOCUS_CHANGE:
        ret = "SDL Window Focus Change";
        break;

    default:
        ret = "Unknown Sub Event";
    }

    return ret;
}

////////////////////////////////
EventDispatcher::EventDispatcher()
{
    // Generate collections
    unsigned int type = UNDEF_EVENT;
    eventListenersMutex.lock();

    while (type < EVENT_TYPE_COUNT) {
        eventListeners[(EventType)type] = new EventListenerCollection();
        type++;
    }

    eventListenersMutex.unlock();
}


EventDispatcher::~EventDispatcher()
{
    eventListeners.clear();
}



void EventDispatcher::HandleEvent(Event *e)
{
    EventListenerCollection *listeners;
    eventListenersMutex.lock();
    listeners = eventListeners[e->type];
    eventListenersMutex.unlock();
    listeners->collectionMutex.lock();

    for (auto it  = listeners->collection.begin();
         it != listeners->collection.end();
         it++) {
        (*it)->HandleEvent(e);
    }

    listeners->collectionMutex.unlock();
}


void EventDispatcher::AddEventListener(EventType type, EventListenerPtr listener)
{
    EventListenerCollection *listeners;
    eventListenersMutex.lock();
    listeners = eventListeners[type];
    eventListenersMutex.unlock();
    listeners->collectionMutex.lock();
    listeners->collection.push_back(listener);
    listeners->collectionMutex.unlock();
}
///////////////////////////////////

EventQueue::~EventQueue()
{
    /*
    for(auto it  = eventQueue.begin();
              it != eventQueue.end();
              it++ )
    {
        delete *it;
    }
    */
}


Event* EventQueue::GetEvent()
{
    std::lock_guard<std::mutex> eventLock(eventQueueMutex);
    Event *event = nullptr;

    if (eventQueue.size() <= 0) {
        return event;
    }

    event = *(eventQueue.begin());
    eventQueue.erase(eventQueue.begin());
    return event;
}



void EventQueue::AddEvent(Event *newEvent)
{
    std::lock_guard<std::mutex> eventLock(eventQueueMutex);
    eventQueue.push_back(newEvent);
}



size_t EventQueue::GetEventCount()
{
    std::lock_guard<std::mutex> eventLock(eventQueueMutex);
    return eventQueue.size();
}


///////////////////////////////////
int main()
{
    system("pause");
    return 0;
}