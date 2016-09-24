// ReflectorService.h

#include "ntservice.h"
#include "..\driver\comm-ioctl.h"

class CReflectorService : public CNTService
{
public:
    CReflectorService();

    virtual BOOL OnInit();
    virtual void Run();
    virtual BOOL OnUserControl(DWORD dwOpcode);
    virtual void OnDeviceEvent(DWORD dwEventType,LPVOID lpEventData);

    BOOL GetRequest(POSR_COMM_CONTROL_REQUEST PRequest,LPOVERLAPPED POverlapped);
    BOOL SendResponse(POSR_COMM_CONTROL_RESPONSE PResponse,LPOVERLAPPED POverlapped);
    BOOL GetRequestSendResponse(POSR_COMM_CONTROL_REQUEST PRequest,
                                POSR_COMM_CONTROL_RESPONSE PResponse,
                                LPOVERLAPPED POverlapped);
    void SaveStatus();

    // Control parameters
    int m_iStartParam;
    int m_iIncParam;

    // Current state
    int m_iState;

    BOOL    m_Connected;
    HANDLE  m_OsrControlHandle;
};
