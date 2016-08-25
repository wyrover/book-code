#include <iostream>
#include <stdexcept>

using namespace std;

// Some device
class Device
{
public:
    Device(int devno)
    {
        if (devno == 2)
            throw runtime_error("Big problem");
    }
    ~Device() {}
private:
    Device();
};

class Broker
{

public:
    Broker(int devno1, int devno2)

try :
        dev1_(Device(devno1)),   // Create these in the initializer
        dev2_(Device(devno2)) {} // list.
    catch (...)
    {
        throw; // Log the message or translate the error here (see
        // the discussion)
    }

    ~Broker() {}

private:
    Broker();
    Device dev1_;
    Device dev2_;
};

int main()
{
    try {
        Broker b(1, 2);
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}