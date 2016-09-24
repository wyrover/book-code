// ntservice.h
//
// Definitions for CNTService
//

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#include "ntservmsg.h" // Event message ids

#define SERVICE_CONTROL_USER 128

#define HANDLEREX 1

class CNTService
{
public:
    CNTService(const WCHAR* szServiceName,const WCHAR* szServiceDescription);
    virtual ~CNTService();
    BOOL ParseStandardArgs(int argc, WCHAR* argv[]);
    BOOL IsInstalled();
    BOOL Install();
    BOOL Uninstall();
    void LogEvent(WORD wType, DWORD dwID,
                  const WCHAR* pszS1 = NULL,
                  const WCHAR* pszS2 = NULL,
                  const WCHAR* pszS3 = NULL);
    BOOL StartService();
    void SetStatus(DWORD dwState);
    BOOL Initialize();
    virtual void Run();
	virtual BOOL OnInit();
    virtual void OnStop();
    virtual void OnInterrogate();
    virtual void OnPause();
    virtual void OnContinue();
    virtual void OnShutdown();
    virtual void OnDeviceEvent(DWORD dwEventType,LPVOID lpEventData);
    virtual BOOL OnUserControl(DWORD dwOpcode);
    void DebugMsg(const WCHAR* pszFormat, ...);
    
    // static member functions
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
#ifndef HANDLEREX
    static void WINAPI Handler(DWORD dwOpcode);
#else   HANDLEREX
    static DWORD WINAPI HandlerEx(DWORD dwOpcode,DWORD dwEventType,LPVOID lpEventData,LPVOID lpContext);
#endif  HANDLEREX

    // data members
    WCHAR m_szServiceName[64];
    WCHAR m_szServiceDescription[256];
    int m_iMajorVersion;
    int m_iMinorVersion;
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_Status;
    BOOL m_bIsRunning;
    BOOL m_bDebugging;

    // static data
    static CNTService* m_pThis; // nasty hack to get object ptr

private:
    HANDLE m_hEventSource;

};

#endif // _NTSERVICE_H_
