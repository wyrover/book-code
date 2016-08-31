#ifndef firewall_h__
#define firewall_h__


#include <string>
#include <netfw.h>
#include <Windows.h>
#include <rpcsal.h>
#include <crtdbg.h>
#include <objbase.h>
#include <oleauto.h>
#include <stdio.h>
#include <netfw.h>

typedef std::basic_string<TCHAR> generic_string;

class Firewall
{
public:
    Firewall();
    ~Firewall();

    BOOL Init();
    VOID Uninit();

    BOOL GetState();
    BOOL TurnOn();
    BOOL TurnOff();

    BOOL GetAppState(generic_string sAppName);
    BOOL AddAppToWall(generic_string sAppName, generic_string sTag);
    BOOL RemoveAppFromWall(generic_string sAppName, generic_string sTag);

    BOOL GetPortState(int nPort);
    BOOL AddPortToWall(int nPort, generic_string sTag);
    BOOL RemovePortToWall(int nPort, generic_string sTag);

private:
    INetFwProfile* m_NetFwProfile;
    int m_nError;
};


#endif // firewall_h__
