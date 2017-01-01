#include "ScreenMode.h"

#include <string>
#include <vector>
#include <stddef.h>
#include <windows.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <LowLevelMonitorConfigurationAPI.h>

using namespace std;

typedef struct GetMonitorHandleByNameData
{
    string name;
    HMONITOR result;
} GetMonitorHandleByNameData;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    GetMonitorHandleByNameData *data = (GetMonitorHandleByNameData *)dwData;

    MONITORINFOEX mon;
    memset(&mon, 0, sizeof(mon));
    mon.cbSize = sizeof(mon);

    if(!GetMonitorInfo(hMonitor, &mon))
        return true;

    if (data->name == mon.szDevice)
    {
        data->result = hMonitor;
        return false;
    }

    return true;
}

HMONITOR GetMonitorHandleByName(string monitorPath)
{
    GetMonitorHandleByNameData data;
    data.name = monitorPath.c_str();
    data.result = NULL;

    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&data);
    return data.result;
}

// Set the specified monitor's orientation.
void SetMonitorOrientation(string monitorPath, DWORD orientation)
{
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    if(!EnumDisplaySettings(monitorPath.c_str(), ENUM_CURRENT_SETTINGS, &dm))
        return;
    if(dm.dmDisplayOrientation == orientation)
        return;

    // If we're switching from landscape to portrait or vice-versa, swap the height and width.
    bool IsLandscape = (orientation == DMDO_DEFAULT || orientation == DMDO_180);
    bool WasLandscape = (dm.dmDisplayOrientation == DMDO_DEFAULT || dm.dmDisplayOrientation == DMDO_180);
    if(IsLandscape != WasLandscape)
        swap(dm.dmPelsHeight, dm.dmPelsWidth);

    dm.dmDisplayOrientation = orientation;
    ChangeDisplaySettings(&dm, 0);
}

// Set the display orientations of all monitors to match their physical orientation.
void SetOrientations()
{
    for(int idx = 0; ; ++idx)
    {
        // EnumDisplayDevices gives us the device names we'll need to pass to ChangeDisplaySettings, and the
        // device name to pass to GetMonitorHandleByName.
        DISPLAY_DEVICE device = {0};
        device.cb = sizeof(device);
        if(!EnumDisplayDevices(NULL, idx, &device, EDD_GET_DEVICE_INTERFACE_NAME))
            break;
        if(!(device.StateFlags & DISPLAY_DEVICE_ACTIVE))
            continue;
        if(device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
            continue;

        // Create the HMONITOR for this display.
        HMONITOR hMonitor = GetMonitorHandleByName(device.DeviceName);
        if (hMonitor == NULL)
            continue;

        DWORD count;
        if(!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &count))
            continue;

        // Get a physical monitor handle for each physical monitor on this display.
        vector<PHYSICAL_MONITOR> monitors;
        monitors.resize(count);
        if(!GetPhysicalMonitorsFromHMONITOR(hMonitor, count, &monitors[0]))
            continue;

        // A display device can have multiple physical monitors.  Use the orientation of the
        // first display that responds to the VCP query.
        DWORD wantedOrientation = -1;
        for(size_t monitor_index = 0; monitor_index < monitors.size(); ++monitor_index)
        {
            HANDLE hPhysicalMonitor = monitors[monitor_index].hPhysicalMonitor;
            DWORD value;
            if(GetVCPFeatureAndVCPFeatureReply(hPhysicalMonitor, 0xAA, NULL, &value, NULL))
            {
                // Convert the VCP orientation to an orientation for the display.
                switch(value)
                {
                case 1: wantedOrientation = DMDO_DEFAULT; break;
                case 2: wantedOrientation = DMDO_90; break;
                case 3: wantedOrientation = DMDO_180; break;
                case 4: wantedOrientation = DMDO_270; break;
                }
                break;
            }

            DestroyPhysicalMonitor(hPhysicalMonitor);
        }

        if(wantedOrientation != -1)
            SetMonitorOrientation(device.DeviceName, wantedOrientation);
    }
}

// Reset all orientations back to landscape.
void ResetOrientations()
{
    for(int idx = 0; ; ++idx)
    {
        DISPLAY_DEVICE device = {0};
        device.cb = sizeof(device);
        if(!EnumDisplayDevices(NULL, idx, &device, EDD_GET_DEVICE_INTERFACE_NAME))
            break;
        if(!(device.StateFlags & DISPLAY_DEVICE_ACTIVE))
            continue;
        if(device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
            continue;

        SetMonitorOrientation(device.DeviceName, DMDO_DEFAULT);
    }
}

/*
 * Copyright (C) 2014 Glenn Maynard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
