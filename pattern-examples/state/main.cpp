#include <iostream>
using namespace std;
class Context;
/// class State -
class State
{
    // Operations
public:
    virtual ~State() {}
    virtual void Handle(Context* pContext) = 0;
protected:
    State() {}
};
/// class Context -
class Context
{
private:
    State* _state;
    // Operations
public:
    Context(State* pState);
    virtual ~Context();
    void Request();
    void ChangeState(State * pState);
};

/// class ConcreteStateA -
class ConcreteStateA : public State
{
    // Operations
public:
    ConcreteStateA();
    virtual ~ConcreteStateA();
    virtual void Handle(Context* pContext);
};
/// class ConcreteStateB -
class ConcreteStateB : public State
{
    // Operations
public:
    ConcreteStateB();
    virtual ~ConcreteStateB();
    virtual void Handle(Context* pContext);
};

/// class ConcreteStateC -
class ConcreteStateC : public State
{
    // Operations
public:
    ConcreteStateC();
    virtual ~ConcreteStateC();
    virtual void Handle(Context* pContext);
};



ConcreteStateA::ConcreteStateA() {}
ConcreteStateA::~ConcreteStateA() {}
void ConcreteStateA::Handle(Context *pContext)
{
    cout << "ConcreteStateA" << endl;
    pContext->ChangeState(new ConcreteStateB());
}
ConcreteStateB::ConcreteStateB() {}
ConcreteStateB::~ConcreteStateB() {}
void ConcreteStateB::Handle(Context *pContext)
{
    cout << "ConcreteStateB" << endl;
    pContext->ChangeState(new ConcreteStateC());
}
ConcreteStateC::ConcreteStateC() {}
ConcreteStateC::~ConcreteStateC() {}
void ConcreteStateC::Handle(Context *pContext)
{
    cout << "ConcreteStateC" << endl;
    pContext->ChangeState(new ConcreteStateA());
}

Context::Context(State* pState)
{
    _state = pState;
}
Context::~Context()
{
}
void Context::Request()
{
    if (NULL != _state) {
        _state->Handle(this);
    }
}
void Context::ChangeState(State *pState)
{
    _state = pState;
}

int main(int argc, char *argv[])
{
    State *pState = new ConcreteStateA();
    Context *pContext = new Context(pState);
    pContext->Request();
    pContext->Request();
    pContext->Request();
    pContext->Request();
    pContext->Request();
    system("pause");
    return 0;
}