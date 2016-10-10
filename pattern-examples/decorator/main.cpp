#include <iostream>
using namespace std;
class Component
{
public:
    Component();
    virtual ~Component();
    virtual void Operation() = 0;
};
class ConcreteComponent : public Component
{
public:
    ConcreteComponent();
    virtual ~ConcreteComponent();
    virtual void Operation();
};
class Decorator : public Component
{
protected:
    Component* _com;
public:
    Decorator(Component* com);
    virtual ~Decorator();
    virtual void Operation();
    void SetComponent(Component* com);
};

class ConcreteDecoratorA : public Decorator
{
public:
    ConcreteDecoratorA(Component* com);
    virtual ~ConcreteDecoratorA();
    virtual void Operation();
private:
    void AddBehavorA();
};
class ConcreteDecoratorB : public Decorator
{
public:
    ConcreteDecoratorB(Component* com);
    virtual ~ConcreteDecoratorB();
    virtual void Operation();
private:
    void AddBehavorB();
};
class ConcreteDecoratorC : public Decorator
{
public:
    ConcreteDecoratorC(Component* com);
    virtual ~ConcreteDecoratorC();
    virtual void Operation();
private:
    void AddBehavorC();
};
class ConcreteDecoratorD : public Decorator
{
public:
    ConcreteDecoratorD(Component* com);
    virtual ~ConcreteDecoratorD();
    virtual void Operation();
private:
    void AddBehavorD();
};
Component::Component()
{
}
Component::~Component()
{
    cout << "~Component" << endl;
}
ConcreteComponent::ConcreteComponent()
{
}
ConcreteComponent::~ConcreteComponent()
{
    cout << "~ConcreteComponent" << endl;
}
void ConcreteComponent::Operation()
{
    cout << "ConcreteComponent::Operation" << endl;
}
Decorator::Decorator(Component* com)
{
    _com = com;
}
Decorator::~Decorator()
{
}
void Decorator::SetComponent(Component* com)
{
    _com = com;
}
void Decorator::Operation()
{
}
ConcreteDecoratorA::ConcreteDecoratorA(Component *com): Decorator(com)
{
}
ConcreteDecoratorA::~ConcreteDecoratorA()
{
    cout << "~ConcreteDecoratorA" << endl;
}
void ConcreteDecoratorA::Operation()
{
    _com->Operation();
    AddBehavorA();
}
void ConcreteDecoratorA::AddBehavorA()
{
    cout << "ConcreteDecoratorA::AddBehaveorA" << endl;
}
ConcreteDecoratorB::ConcreteDecoratorB(Component *com): Decorator(com)
{
}
ConcreteDecoratorB::~ConcreteDecoratorB()
{
    cout << "~ConcreteDecoratorB" << endl;
}
void ConcreteDecoratorB::Operation()
{
    _com->Operation();
    AddBehavorB();
}
void ConcreteDecoratorB::AddBehavorB()
{
    cout << "ConcreteDecoratorB::AddBehaveorB" << endl;
}
ConcreteDecoratorC::ConcreteDecoratorC(Component *com): Decorator(com)
{
}
ConcreteDecoratorC::~ConcreteDecoratorC()
{
    cout << "~ConcreteDecoratorC" << endl;
}
void ConcreteDecoratorC::Operation()
{
    _com->Operation();
    AddBehavorC();
}
void ConcreteDecoratorC::AddBehavorC()
{
    cout << "ConcreteDecoratorC::AddBehaveorC" << endl;
}
ConcreteDecoratorD::ConcreteDecoratorD(Component *com): Decorator(com)
{
}
ConcreteDecoratorD::~ConcreteDecoratorD()
{
    cout << "~ConcreteDecoratorD" << endl;
}
void ConcreteDecoratorD::Operation()
{
    _com->Operation();
    AddBehavorD();
}
void ConcreteDecoratorD::AddBehavorD()
{
    cout << "ConcreteDecoratorD::AddBehaveorD" << endl;
}
int main(int argc, char *argv[])
{
    Component* pCom = new ConcreteComponent();
    Decorator* pDec = NULL;
    pDec = new ConcreteDecoratorA(pCom);
    pDec = new ConcreteDecoratorB(pDec);
    pDec = new ConcreteDecoratorC(pDec);
    pDec = new ConcreteDecoratorD(pDec);
    pDec->Operation();
    cout << "------------------------------------------" << endl;
    system("pause");
    return 0;
}