// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int     nRetCode = 0;
    HANDLE  h = INVALID_HANDLE_VALUE;
    char    dataBuffer[65535];

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
	}

    h = CreateFile("\\\\.\\OSRcommData",GENERIC_READ|GENERIC_WRITE,
                            NULL,NULL,OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,NULL);

    if(h == INVALID_HANDLE_VALUE) {
    
        nRetCode = GetLastError();
        printf("Error opening Data Device. %d.\n",nRetCode);
        return nRetCode;

    }

    for(DWORD index = 0; index < 10; index++) {

        DWORD bytesWritten,bytesRead;

        memset(&dataBuffer[0],index,sizeof(dataBuffer));

        if(!WriteFile(h,&dataBuffer[0],sizeof(dataBuffer),&bytesWritten,NULL)) {

            nRetCode = GetLastError();
            CloseHandle(h);
            printf("Error Writing to Data Device. %d.\n",nRetCode);
            return nRetCode;

        }

        memset(&dataBuffer[0],index,sizeof(dataBuffer));

        if(!ReadFile(h,&dataBuffer[0],sizeof(dataBuffer),&bytesRead,NULL)) {

            nRetCode = GetLastError();
            CloseHandle(h);
            printf("Error Writing to Data Device. %d.\n",nRetCode);
            return nRetCode;

        }

    }


    CloseHandle(h);


	return nRetCode;
}


