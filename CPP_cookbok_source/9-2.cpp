#include <iostream>
#include <stdexcept>

using namespace std;

class Device
{
public:
    Device(int devno)
    {
        if (devno == 2)
            throw runtime_error("Big problem");
    }
    ~Device() {}
};

class Broker
{

public:
    Broker(int devno1, int devno2) :
        dev1_(NULL), dev2_(NULL)
    {
        try {
            dev1_ = new Device(devno1);  // Enclose the creation of heap
            dev2_ = new Device(devno2);  // objects in a try block...
        } catch (...) {
            delete dev1_;                // ...clean up and rethrow if
            throw;                       // something goes wrong.
        }
    }
    ~Broker()
    {
        delete dev1_;
        delete dev2_;
    }

private:
    Broker();
    Device* dev1_;
    Device* dev2_;
};

int main()
{
    try {
        Broker b(1, 2);
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}