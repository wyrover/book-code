#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class AbstractConfigFile
{

public:
    virtual ~AbstractConfigFile() {}

    virtual void getKey(const string& header,
                        const string& key,
                        string& val) const = 0;
    virtual void exists(const string& header,
                        const string& key,
                        string& val) const = 0;
};

class TXTConfigFile : public AbstractConfigFile
{

public:
    TXTConfigFile() : in_(NULL) {}
    TXTConfigFile(istream& in) : in_(&in) {}
    virtual ~TXTConfigFile() {}

    virtual void getKey(const string& header,
                        const string& key,
                        string& val) const {}
    virtual void exists(const string& header,
                        const string& key,
                        string& val) const {}

protected:
    istream* in_;
};

class MyAppClass
{
public:
    MyAppClass() : config_(NULL) {}
    ~MyAppClass() {}
    void setConfigObj(const AbstractConfigFile* p)
    {
        config_ = p;
    }
    void myMethod();

private:
    const AbstractConfigFile* config_;
};

void MyAppClass::myMethod()
{
    string val;
    config_->getKey("Foo", "Bar", val);
    // ...
}

int main()
{
    ifstream in("foo.txt");
    TXTConfigFile cfg(in);
    MyAppClass m;
    m.setConfigObj(&cfg);
    m.myMethod();
}