#include <memory>
#include <vector>
#include <iostream>

template<typename Event>
class Dispatcher
{
public:
    class Listener
    {
    public:
        virtual void OnEvent(Event& event, Dispatcher& sender) {};
    };

private:
    typedef std::shared_ptr<Listener> ListenerPtr;
    typedef std::vector<ListenerPtr> Listeners;
    Listeners _listeners;

public:
    void Reg(ListenerPtr listener)
    {
        if (_listeners.end() != std::find(_listeners.begin(), _listeners.end(), listener))
            return;

        _listeners.push_back(listener);
    }

    void Unreg(ListenerPtr listener)
    {
        Listeners::iterator iter = std::find(_listeners.begin(), _listeners.end(), listener);

        if (_listeners.end() == iter)
            return;

        _listeners.erase(iter);
    }

    void Dispatch(Event& event)
    {
        for (Listeners::iterator iter = _listeners.begin(); iter != _listeners.end(); ++iter)
            (*iter)->OnEvent(event, *this);
    }
};

struct SomeEvent {
    int someParam;
    SomeEvent(int someParam) : someParam(someParam) {}
};

class SomeDispatcher : public Dispatcher<SomeEvent>
{
};

struct OtherEvent {
    int otherParam;
    OtherEvent(int otherParam) : otherParam(otherParam) {}
};

class OtherDispatcher :
    public SomeDispatcher, public Dispatcher<OtherEvent>
{
};

class Consumer :
    public Dispatcher<SomeEvent>::Listener,
    public Dispatcher<OtherEvent>::Listener
{
    virtual void OnEvent(SomeEvent& event, Dispatcher<SomeEvent>& sender)
    {
        std::cout << "OnEvent SomeEvent " << event.someParam << std::endl;
    }

    virtual void OnEvent(OtherEvent& event, Dispatcher<OtherEvent>& sender)
    {
        std::cout << "OnEvent OtherEvent " << event.otherParam << std::endl;
    }
};


int main(int argc, char **argv)
{
    OtherDispatcher dispatcher;
    std::shared_ptr<Consumer> consumer(new Consumer());
    dispatcher.Dispatcher<SomeEvent>::Reg(consumer);
    dispatcher.Dispatcher<OtherEvent>::Reg(consumer);
    dispatcher.Dispatcher<SomeEvent>::Dispatch(SomeEvent(1));
    dispatcher.Dispatcher<OtherEvent>::Dispatch(OtherEvent(2));
    dispatcher.Dispatcher<SomeEvent>::Unreg(consumer);
    dispatcher.Dispatcher<SomeEvent>::Dispatch(SomeEvent(3));
    dispatcher.Dispatcher<OtherEvent>::Dispatch(OtherEvent(4));
    system("pause");
    return 0;
}