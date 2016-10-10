#include <iostream>
#include <string>
using namespace std;
class Mediator;
class Colleage
{
public:
    ~Colleage();
    void SetMediator(Mediator* pMediator);
    virtual void SendMsg(string msg) = 0;
    virtual void GetMsg(string msg) = 0;
protected:
    Colleage(Mediator* pMediator);
    Mediator* _mediator;
};
Colleage::Colleage(Mediator* pMediator)
{
    _mediator = pMediator;
}
Colleage::~Colleage()
{
}
void Colleage::SetMediator(Mediator* pMediator)
{
    _mediator = pMediator;
}
class Mediator
{
    // Operations
public:
    ~Mediator();
    virtual void SendMsg(string s, Colleage* colleage) = 0;
protected:
    Mediator();
};
Mediator::Mediator()
{
}
Mediator::~Mediator()
{
}
/// class ConcreteMediator -
class ConcreteMediator : public Mediator
{
    // Attributes
private:
    Colleage* m_ColleageA;
    Colleage* m_ColleageB;
    // Operations
public:
    ConcreteMediator();
    virtual ~ConcreteMediator();
    virtual void SendMsg(string s, Colleage* p);
    void SetColleageA(Colleage* p);
    void SetColleageB(Colleage* p);
};

ConcreteMediator::ConcreteMediator()
{
}
ConcreteMediator::~ConcreteMediator()
{
}
void ConcreteMediator::SetColleageA(Colleage* p)
{
    m_ColleageA = p;
}
void ConcreteMediator::SetColleageB(Colleage* p)
{
    m_ColleageB = p;
}
void ConcreteMediator::SendMsg(string s, Colleage* p)
{
    if (p == m_ColleageA)
        m_ColleageB->GetMsg(s);

    if (p == m_ColleageB)
        m_ColleageA->GetMsg(s);
}

class ConcreteColleageA : public Colleage
{
    // Operations
public:
    ConcreteColleageA(Mediator* pMediator);
    ~ConcreteColleageA();
    virtual void SendMsg(string msg);
    virtual void GetMsg(string msg);
};

ConcreteColleageA::ConcreteColleageA(Mediator* pMediator): Colleage(pMediator)
{
}
ConcreteColleageA::~ConcreteColleageA()
{
}
void ConcreteColleageA::SendMsg(string msg)
{
    _mediator->SendMsg(msg, this);
}
void ConcreteColleageA::GetMsg(string msg)
{
    cout << "ConcreteColleageA Receive::" << msg << endl;
}

/// class ConcreteColleageB -
class ConcreteColleageB : public Colleage
{
    // Operations
public:
    ConcreteColleageB(Mediator* pMediator);
    ~ConcreteColleageB();
    virtual void SendMsg(string msg);
    virtual void GetMsg(string msg);
};
ConcreteColleageB::ConcreteColleageB(Mediator* pMediator): Colleage(pMediator)
{
}
ConcreteColleageB::~ConcreteColleageB()
{
}
void ConcreteColleageB::SendMsg(string msg)
{
    _mediator->SendMsg(msg, this);
}
void ConcreteColleageB::GetMsg(string msg)
{
    cout << "ConcreteColleageB Receive::" << msg << endl;
}
int main(int argc, char *argv[])
{
    ConcreteMediator *pMediator = new ConcreteMediator();
    Colleage* p1 = new ConcreteColleageA(pMediator);
    Colleage* p2 = new ConcreteColleageB(pMediator);
    pMediator->SetColleageA(p1);
    pMediator->SetColleageB(p2);
    p1->SendMsg("xxx");
    p2->SendMsg("ooo");
    system("pause");
    return 0;
}