#include <iostream>
using namespace std;

/// class AdstractionImplement -
class AbstractionImplement
{
    // Operations
public:
    virtual ~AbstractionImplement();
    virtual void Operation() = 0;
protected:
    AbstractionImplement();
};

/// class Adstraction -
class Abstraction
{
    // Operations
public:
    virtual ~Abstraction();
    virtual void Operation() = 0;
protected:
    Abstraction();
};
/// class RefinedAdstractionA -
class RefinedAbstractionA : public Abstraction
{
    // Attributes
private:
    AbstractionImplement* _imp;
    // Operations
public:
    ~RefinedAbstractionA();
    RefinedAbstractionA(AbstractionImplement* imp);
    virtual void Operation();
};

/// class RefinedAdstractionB -
class RefinedAbstractionB : public Abstraction
{
    // Attributes
public:
    AbstractionImplement* _imp;
    // Operations
public:
    ~RefinedAbstractionB();
    RefinedAbstractionB(AbstractionImplement* imp);
    virtual void Operation();
};

/// class ConcreteAbstractImplementA -
class ConcreteAbstractImplementA : public AbstractionImplement
{
    // Operations
public:
    virtual ~ConcreteAbstractImplementA();
    virtual void Operation();
    ConcreteAbstractImplementA();
};

/// class ConcreteAbstractImplementB -
class ConcreteAbstractImplementB : public AbstractionImplement
{
    // Operations
public:
    ConcreteAbstractImplementB();
    virtual void Operation();
    virtual ~ConcreteAbstractImplementB();
};

AbstractionImplement::AbstractionImplement()
{
}
AbstractionImplement::~AbstractionImplement()
{
}

Abstraction::Abstraction()
{
}
Abstraction::~Abstraction()
{
}
RefinedAbstractionA::RefinedAbstractionA(AbstractionImplement *imp)
{
    _imp = imp;
}
RefinedAbstractionA::~RefinedAbstractionA()
{
    delete _imp;
    _imp = NULL;
}
void RefinedAbstractionA::Operation()
{
    cout << "RefinedAbstractionA::Operation" << endl;
    _imp->Operation();
}
RefinedAbstractionB::RefinedAbstractionB(AbstractionImplement *imp)
{
    _imp = imp;
}
RefinedAbstractionB::~RefinedAbstractionB()
{
    delete _imp;
    _imp = NULL;
}
void RefinedAbstractionB::Operation()
{
    cout << "RefinedAbstractionB::Operation" << endl;
    _imp->Operation();
}
ConcreteAbstractImplementA::ConcreteAbstractImplementA()
{
}
ConcreteAbstractImplementA::~ConcreteAbstractImplementA()
{
}
void ConcreteAbstractImplementA::Operation()
{
    cout << "ConcreteAbstractionImplementA Operation" << endl;
}
ConcreteAbstractImplementB::ConcreteAbstractImplementB()
{
}
ConcreteAbstractImplementB::~ConcreteAbstractImplementB()
{
}
void ConcreteAbstractImplementB::Operation()
{
    cout << "ConcreteBbstractionImplementB Operation" << endl;
}
int main(int argc, char *argv[])
{
    AbstractionImplement* imp = new ConcreteAbstractImplementA();
    Abstraction* abs = new RefinedAbstractionA(imp);
    abs->Operation();
    cout << "------------------------------------------" << endl;
    AbstractionImplement *imp1 = new ConcreteAbstractImplementB();
    Abstraction* abs1 = new RefinedAbstractionA(imp1);
    abs1->Operation();
    cout << "------------------------------------------" << endl;
    AbstractionImplement* imp2 = new ConcreteAbstractImplementA();
    Abstraction* abs2 = new RefinedAbstractionB(imp2);
    abs2->Operation();
    cout << "------------------------------------------" << endl;
    AbstractionImplement *imp3 = new ConcreteAbstractImplementB();
    Abstraction* abs3 = new RefinedAbstractionB(imp3);
    abs3->Operation();
    cout << "------------------------------------------" << endl;
    system("pause");
    return 0;
}