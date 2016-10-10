#include <iostream>
#include <string>
#include <list>
#include <algorithm>
class Subject;
using namespace std;

class Subject;
/// class Observer -
class Observer
{
    // Operations
public:
    ~Observer();
    virtual void Update(Subject* pSubject) = 0;
protected:
    Observer();
};
Observer::Observer()
{
}
Observer::~Observer()
{
}
/// class Subject -
class Subject
{
    // Associations
    // Attributes
public:
    list<Observer*> m_lst;
private:
    string m_state;
    // Operations
public:
    ~Subject();
    void Notify();
    void Attach(Observer* pObserver);
    void Detach(Observer* pObserver);
    void SetState(string state);
    string GetState();
protected:
    Subject();
};

Subject::Subject()
{
}
Subject::~Subject()
{
}
void Subject::Notify()
{
    list<Observer*>::iterator iter = m_lst.begin();

    for (; iter != m_lst.end(); iter++) {
        (*iter)->Update(this);
    }
}
void Subject::Attach(Observer * pObserver)
{
    m_lst.push_back(pObserver);
    cout << "Attach an Obsercer" << endl;
}
void Subject::Detach(Observer* pObserver)
{
    list<Observer*>::iterator iter;
    iter = find(m_lst.begin(), m_lst.end(), pObserver);

    if (iter != m_lst.end()) {
        m_lst.erase(iter);
    }

    cout << "Detach an Observer" << endl;
}
void Subject::SetState(string state)
{
    m_state = state;
}
string Subject::GetState()
{
    return m_state;
}
/// class ConcreteObserverA -
class ConcreteObserverA : public Observer
{
    // Operations
public:
    ConcreteObserverA();
    ~ConcreteObserverA();
    virtual void Update(Subject* pSubject);
private:
    string m_state;
};
ConcreteObserverA::ConcreteObserverA()
{
}
ConcreteObserverA::~ConcreteObserverA()
{
}
void ConcreteObserverA::Update(Subject* pSubject)
{
    m_state = pSubject->GetState();
    cout << "The ConcreteObserverA is " << m_state << endl;
}
/// class ConcreteObserverB -
class ConcreteObserverB : public Observer
{
    // Operations
public:
    ConcreteObserverB();
    ~ConcreteObserverB();
    virtual void Update(Subject* pSubject);
private:
    string m_state;
};
ConcreteObserverB::ConcreteObserverB()
{
}
ConcreteObserverB::~ConcreteObserverB()
{
}
void ConcreteObserverB::Update(Subject* pSubject)
{
    m_state = pSubject->GetState();
    cout << "The ConcreteObserverB is " << m_state << endl;
}

/// class ConcreteSubjectA -
class ConcreteSubjectA : public Subject
{
    // Operations
public:
    ConcreteSubjectA();
    ~ConcreteSubjectA();
};

ConcreteSubjectA::ConcreteSubjectA()
{
}
ConcreteSubjectA::~ConcreteSubjectA()
{
}

/// class ConcreteSubjectB -
class ConcreteSubjectB : public Subject
{
    // Operations
public:
    ConcreteSubjectB();
    ~ConcreteSubjectB();
};

ConcreteSubjectB::ConcreteSubjectB()
{
}
ConcreteSubjectB::~ConcreteSubjectB()
{
}
int main(int argc, char *argv[])
{
    Observer* p1 = new ConcreteObserverA();
    Observer* p2 = new ConcreteObserverB();
    Observer* p3 = new ConcreteObserverA();
    Subject* pSubject = new ConcreteSubjectA();
    pSubject->Attach(p1);
    pSubject->Attach(p2);
    pSubject->Attach(p3);
    pSubject->SetState("old");
    pSubject->Notify();
    cout << "--------------------------------------" << endl;
    pSubject->SetState("new");
    pSubject->Detach(p3);
    pSubject->Notify();
    system("pause");
    return 0;
}