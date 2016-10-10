#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Flyweight
{
    // Attributes
private:
    string _instrinsicStatic;
    // Operations
public:
    ~Flyweight();
    virtual void Operation(const string& extrinsicState) = 0;
    string GetInstrinsicState();
protected:
    Flyweight(string intrinsicState);
};
Flyweight::Flyweight(string intrinsicState)
{
    _instrinsicStatic = intrinsicState;
}
Flyweight::~Flyweight()
{
}
string Flyweight::GetInstrinsicState()
{
    return _instrinsicStatic;
}

class ConcreteFlyweight : public Flyweight
{
    // Attributes
private:
    string _instrinsicStatic;
    // Operations
public:
    ConcreteFlyweight(string intrinsicState);
    ~ConcreteFlyweight();
    virtual void Operation(const string& extrinsicState);
};
ConcreteFlyweight::ConcreteFlyweight(string intrinsicState): Flyweight(intrinsicState)
{
}
ConcreteFlyweight::~ConcreteFlyweight()
{
}
void ConcreteFlyweight::Operation(const string& extrinsicState)
{
    cout << GetInstrinsicState() << endl;
    cout << extrinsicState << endl;
}
/// class UnsharedConcreteFlyweight -
class UnsharedConcreteFlyweight : public Flyweight
{
    // Attributes
private:
    string _instrinsicStatic;
    // Operations
public:
    UnsharedConcreteFlyweight(string intrinsicState);
    ~UnsharedConcreteFlyweight();
    virtual void Operation(const string& extrinsicState);
};

UnsharedConcreteFlyweight::UnsharedConcreteFlyweight(string intrinsicState): Flyweight(intrinsicState)
{
}
UnsharedConcreteFlyweight::~UnsharedConcreteFlyweight()
{
}
void UnsharedConcreteFlyweight::Operation(const string& extrinsicState)
{
    cout << extrinsicState << endl;
}

/// class FlyweightFactory -
class FlyweightFactory
{
    // Attributes
public:
    vector<Flyweight*> m_vecFly;
    // Operations
public:
    FlyweightFactory();
    ~FlyweightFactory();
    Flyweight* GetFlyweight(string key);
    void GetFlyweightCount();
};

FlyweightFactory::FlyweightFactory()
{
}
FlyweightFactory::~FlyweightFactory()
{
}
Flyweight* FlyweightFactory::GetFlyweight(string key)
{
    vector<Flyweight*>::iterator iter = m_vecFly.begin();

    for (; iter != m_vecFly.end(); iter++) {
        if ((*iter)->GetInstrinsicState() == key) {
            return *iter;
        }
    }

    Flyweight* fly = new ConcreteFlyweight(key);
    m_vecFly.push_back(fly);
    return fly;
}
void FlyweightFactory::GetFlyweightCount()
{
    cout << m_vecFly.size() << endl;
}

int main(int argc, char *argv[])
{
    string extrinsicState = "ext";
    FlyweightFactory* fc = new FlyweightFactory();
    Flyweight* fly = fc->GetFlyweight("hello");
    Flyweight* fly1 = fc->GetFlyweight("hello");
    fly->Operation(extrinsicState);
    fc->GetFlyweightCount();
    system("pause");
    return 0;
}