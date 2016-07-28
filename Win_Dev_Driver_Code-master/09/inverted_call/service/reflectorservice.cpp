// ReflectorService.cpp

#include "stdafx.h"
#include "appsrv.h"
#include "reflectorservice.h"
#include <dbt.h>
#include <devguid.h>
#include <winioctl.h>

typedef struct _CommRequest {

    OSR_COMM_CONTROL_REQUEST    CommRequest;
    char                        CommRequestBuffer[65536];

} COMMREQUEST, *PCOMMREQUEST;

typedef struct _CommResponse {

    OSR_COMM_CONTROL_RESPONSE  CommResponse;
    char                       CommResponseBuffer[65536];

} COMMRESPONSE, *PCOMMRESPONSE;



CReflectorService::CReflectorService() :CNTService(L"OsrReflect",L"OSR Reflector Service"),
                                        m_Connected(FALSE),m_OsrControlHandle(INVALID_HANDLE_VALUE)
{
	  m_iStartParam = 0;
	  m_iIncParam = 1;
	  m_iState = m_iStartParam;

}

BOOL CReflectorService::OnInit()
{
	  // Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	  WCHAR szKey[1024];
	  wcscpy(szKey, L"SYSTEM\\CurrentControlSet\\Services\\");
	  wcscat(szKey, m_szServiceName);
	  wcscat(szKey, L"\\Parameters");
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     szKey,
                     0,
                     KEY_QUERY_VALUE,
                     &hkey) == ERROR_SUCCESS) {
        // Yes we are installed
        DWORD dwType = 0;
        DWORD dwSize = sizeof(m_iStartParam);
        RegQueryValueEx(hkey,
                        L"Start",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iStartParam,
                        &dwSize);
        dwSize = sizeof(m_iIncParam);
        RegQueryValueEx(hkey,
                        L"Inc",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iIncParam,
                        &dwSize);
        RegCloseKey(hkey);
    }

	  // Set the initial state
	  m_iState = m_iStartParam;


    //
    // Open up the OSRControl Device.   This will be the device that 
    // we receive the requests from.
    //

    m_OsrControlHandle = CreateFile(L"\\\\.\\OsrcommControl",GENERIC_READ|GENERIC_WRITE,
                                    NULL,NULL,OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);

    if(m_OsrControlHandle != INVALID_HANDLE_VALUE) {

      m_Connected = TRUE;

    }

	return TRUE;
}

void CReflectorService::Run()
{
    OVERLAPPED                  CommRequestOverlapped;
    COMMREQUEST                 CommRequest;
    COMMRESPONSE                CommResponse;
    BOOL                        ret;

    DebugMsg(L"CReflectorService::Run Entered.");

    //DebugBreak();

    while (m_bIsRunning) {

        // Sleep for a while
        DebugMsg(L"CReflectorService is sleeping (%lu)...", m_iState);
        Sleep(1000);

        if(!m_Connected) {

            m_OsrControlHandle = CreateFile(L"\\\\.\\OsrcommControl",GENERIC_READ|GENERIC_WRITE,
                                    NULL,NULL,OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);

            if(m_OsrControlHandle != INVALID_HANDLE_VALUE) {

                m_Connected = TRUE;

            } else {

                DebugMsg(L"CReflectorService Can't Connect to Control (%lu)...", GetLastError());
                continue;

            }

        }

        // Update the current state
        m_iState += m_iIncParam;

        //
        // See if there is any work to do.....
        //

        memset(&CommRequestOverlapped,0,sizeof(CommRequestOverlapped));

        CommRequestOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

        if(CommRequestOverlapped.hEvent == INVALID_HANDLE_VALUE) {

            continue;

        }

        memset(&CommRequest,0,sizeof(CommRequest));

        CommRequest.CommRequest.RequestBuffer = &CommRequest.CommRequestBuffer[0];
        CommRequest.CommRequest.RequestBufferLength = sizeof(CommRequest.CommRequestBuffer);

        DebugMsg(L"CReflectorService: Getting Request: CommRequest RequestBuffer %x, Length %x", 
                 CommRequest.CommRequest.RequestBuffer,
                 CommRequest.CommRequest.RequestBufferLength);

        ret = GetRequest(&CommRequest.CommRequest,&CommRequestOverlapped);

        if(!ret) {

            CloseHandle(CommRequestOverlapped.hEvent);
            continue;

        } else {

            BOOL running = TRUE;

            while(!HasOverlappedIoCompleted(&CommRequestOverlapped)) {

                if(!m_bIsRunning) {

                    goto exitStageLeft;                    

                } 

                Sleep(100);

            }

            if(running) {

                DWORD   bytesTransferred;

                //
                // Process Request.
                //
                running = GetOverlappedResult(m_OsrControlHandle,&CommRequestOverlapped,&bytesTransferred,FALSE);
                
                ASSERT(running);

                //DebugBreak();

                DebugMsg(L"CReflectorService: GetRequest Completed"); 
                DebugMsg(L"CReflectorService: GetRequest Completed: Id= %x, Type = %x, CommRequest RequestBuffer %x, Length %x", 
                         CommRequest.CommRequest.RequestID,
                         CommRequest.CommRequest.RequestType,
                         CommRequest.CommRequest.RequestBuffer,
                         CommRequest.CommRequest.RequestBufferLength);

                CommResponse.CommResponse.RequestID = CommRequest.CommRequest.RequestID;
                CommResponse.CommResponse.ResponseType = CommRequest.CommRequest.RequestType;
                CommResponse.CommResponse.ResponseBuffer = &CommResponse.CommResponseBuffer[0];
                CommResponse.CommResponse.ResponseBufferLength = CommRequest.CommRequest.RequestBufferLength;

                memcpy(&CommResponse.CommResponseBuffer[0],
                       &CommRequest.CommRequestBuffer[0],
                       CommRequest.CommRequest.RequestBufferLength);

                ResetEvent(CommRequestOverlapped.hEvent);
                
                DebugMsg(L"CReflectorService: Sending Response: Id= %x, Type = %x, CommRequest RequestBuffer %x, Length %x", 
                         CommResponse.CommResponse.RequestID,
                         CommResponse.CommResponse.ResponseType,
                         CommResponse.CommResponse.ResponseBuffer,
                         CommResponse.CommResponse.ResponseBufferLength);

                //
                // Send Response.
                //
                ret = SendResponse(&CommResponse.CommResponse,&CommRequestOverlapped);

                if(!ret) {

                    //DebugBreak();
                    CloseHandle(CommRequestOverlapped.hEvent);
                    continue;

                } else {

                    BOOL running = TRUE;

                    while(!HasOverlappedIoCompleted(&CommRequestOverlapped)) {

                        if(!m_bIsRunning) {

                            goto exitStageLeft;                    

                        } 

                        Sleep(100);

                    }

                    DebugMsg(L"CReflectorService: SendResponse Completed.\n");

                    CloseHandle(CommRequestOverlapped.hEvent);

                }

            }
        }
    }

exitStageLeft:

    if(m_OsrControlHandle != INVALID_HANDLE_VALUE) {

        CloseHandle(m_OsrControlHandle);
        m_OsrControlHandle = INVALID_HANDLE_VALUE;
        m_Connected = FALSE;

    }

}

void CReflectorService::OnDeviceEvent(DWORD dwEventType,LPVOID lpEventData)
{
    PDEV_BROADCAST_DEVICEINTERFACE pBroadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) lpEventData;

    switch(dwEventType) {

        case DBT_DEVICEARRIVAL:
            DebugMsg(L"CReflectorService::OnDeviceEvent - Arrival %ls",&pBroadcastInterface->dbcc_name[0]);
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            DebugMsg(L"CReflectorService::OnDeviceEvent - Removal %ls",&pBroadcastInterface->dbcc_name[0]);
            break;

        default:
            break;

    }
}

// Process user control requests
BOOL CReflectorService::OnUserControl(DWORD dwOpcode)
{
    switch (dwOpcode) {
    case SERVICE_CONTROL_USER + 0:

        // Save the current status in the registry
        SaveStatus();
        return TRUE;

    default:
        break;
    }
    return FALSE; // say not handled
}

// Save the current status in the registry
void CReflectorService::SaveStatus()
{
    DebugMsg(L"Saving current status");
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
    HKEY hkey = NULL;
	  WCHAR szKey[1024];
	  wcscpy(szKey, L"SYSTEM\\CurrentControlSet\\Services\\");
	  wcscat(szKey, m_szServiceName);
	  wcscat(szKey, L"\\Status");
    DWORD dwDisp;
	  DWORD dwErr;
    DebugMsg(L"Creating key: %s", szKey);
    dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			    0,
                   			    L"",
                   			    REG_OPTION_NON_VOLATILE,
                   			    KEY_WRITE,
                   			    NULL,
                   			    &hkey,
                   			    &dwDisp);

	  if (dwErr != ERROR_SUCCESS) {
		  DebugMsg(L"Failed to create Status key (%lu)", dwErr);
		  return;
	  }	

      // Set the registry values
	  DebugMsg(L"Saving 'Current' as %ld", m_iState); 
    RegSetValueEx(hkey,
                  L"Current",
                  0,
                  REG_DWORD,
                  (BYTE*)&m_iState,
                  sizeof(m_iState));


    // Finished with key
    RegCloseKey(hkey);

}

BOOL CReflectorService::GetRequest(POSR_COMM_CONTROL_REQUEST PRequest,LPOVERLAPPED POverlapped)
{
    BOOL    status;
    DWORD   bytesReturned;

    DebugMsg(L"CReflectorService::GetRequest - Entered");

    status = DeviceIoControl(m_OsrControlHandle,OSR_COMM_CONTROL_GET_REQUEST,
                             PRequest,sizeof(OSR_COMM_CONTROL_REQUEST),
                             PRequest,sizeof(OSR_COMM_CONTROL_REQUEST),
                             &bytesReturned,
                             POverlapped);

    if(!status) {

        DWORD error = GetLastError();

        if(error != ERROR_IO_PENDING) {

            DebugMsg(L"CReflectorService::GetRequest - Status %x",GetLastError());
            return FALSE;

        } else {

            DebugMsg(L"CReflectorService::GetRequest Pending");

        }

    }

    DebugMsg(L"CReflectorService::GetRequest - Exit.");

    return TRUE;                             
}

BOOL CReflectorService::SendResponse(POSR_COMM_CONTROL_RESPONSE PResponse,LPOVERLAPPED POverlapped)
{
    BOOL    status;
    DWORD   bytesReturned;

    DebugMsg(L"CReflectorService::SendResponse - Entered");

    status = DeviceIoControl(m_OsrControlHandle,OSR_COMM_CONTROL_SEND_RESPONSE,
                             PResponse,sizeof(OSR_COMM_CONTROL_RESPONSE),
                             NULL,0,
                             &bytesReturned,
                             POverlapped);

    if(!status) {

        DWORD error = GetLastError();

        if(error != ERROR_IO_PENDING) {

            DebugMsg(L"CReflectorService::SendResponse - Status %x",GetLastError());
            return FALSE;

        } else {

            DebugMsg(L"CReflectorService::SendResponse Pending");

        }

    }

    DebugMsg(L"CReflectorService::SendResponse - Exit.");

    return TRUE;                             
}

BOOL CReflectorService::GetRequestSendResponse(POSR_COMM_CONTROL_REQUEST PRequest,
                                               POSR_COMM_CONTROL_RESPONSE PResponse,
                                               LPOVERLAPPED POverlapped)
{
    BOOL    status;
    DWORD   bytesReturned;

    DebugMsg(L"CReflectorService::GetRequestSendResponse - Entered");

    status = DeviceIoControl(m_OsrControlHandle,OSR_COMM_CONTROL_GET_AND_SEND,
                             PResponse,sizeof(OSR_COMM_CONTROL_RESPONSE),
                             PRequest,sizeof(OSR_COMM_CONTROL_REQUEST),
                             &bytesReturned,
                             POverlapped);

    if(!status) {

        DWORD error = GetLastError();

        if(error != ERROR_IO_PENDING) {

            DebugMsg(L"CReflectorService::GetRequestSendResponse - Status %x",GetLastError());
            return FALSE;

        } else {

            DebugMsg(L"CReflectorService::GetRequestSendResponse Pending");

        }
    }

    DebugMsg(L"CReflectorService::GetRequestSendResponse - Exit.");

    return TRUE;                             
}
