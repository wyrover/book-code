// Devices.h
#ifndef DEVICES_H__
#define DEVICES_H__
#include <string>
#include <list>

namespace hardware
{
class Device
{
public:
    Device() : uptime_(0), status_("unknown") {}
    unsigned long getUptime() const;
    std::string getStatus() const;
    void reset();
private:
    unsigned long uptime_;
    std::string status_;
};

class DeviceMgr
{
public:
    void getDeviceIds(std::list<std::string>& ids) const;
    Device getDevice(const std::string& id) const;
    // Other stuff...
};
}

#endif // DEVICES_H__

// Devices.cpp
#include "Devices.h"
#include <string>
#include <list>

namespace hardware
{
using std::string;
using std::list;
unsigned long Device::getUptime() const
{
    return (uptime_);
}
string Device::getStatus() const
{
    return (status_);
}

void DeviceMgr::getDeviceIds(list<string>& ids) const
{
}
Device DeviceMgr::getDevice(const string& id) const
{
    Device d;
    return (d);
}
}

// DeviceWidget.h
#ifndef DEVICEWIDGET_H_ _
#define DEVICEWIDGET_H_ _
#include "Devices.h"

namespace ui
{
class Widget
{
    /* ... */
};
class DeviceWidget : public Widget
{
public:
    DeviceWidget(const hardware::Device& dev) : device_(dev) {}
    // Some other stuff
protected:
    hardware::Device device_;
};
}

#endif // DEVICEWIDGET_H_ _

// main.cpp
#include <iostream>
#include "DeviceWidget.h"
#include "Devices.h"

int main()
{
    hardware::Device d;
    ui::DeviceWidget myWidget(d);
    // ...
}
