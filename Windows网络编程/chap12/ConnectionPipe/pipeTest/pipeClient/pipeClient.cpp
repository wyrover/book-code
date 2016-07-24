// pipeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pipeClient.h"

#include "../../commonIPC/spIPCComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;
void PrintHelp(void)
{
	printf("«Îœ»‘À–– pipeServer.exe\n");
	printf("Command :\n");
	printf("1 : test Send/Recv\n");
	printf("2 : test Send\n");
	printf("3 : test RecvWithTimeOut\n");
	printf("4 : test TestAndRecv\n");
	printf("0 : Exit\n\n");
	printf("press the key and press Enter\n\n");
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		HANDLE hP = SPCreateClientPipe("\\\\.\\pipe\\tp",10);
		//SOCKET sComm = SPCreateConnectSocket(argv[1],9444);
		ASSERT(hP != INVALID_HANDLE_VALUE);
		CIPCNamedPipe s(hP,TRUE);
		BYTE bRead[100];
		BYTE bWrite[100];
		memset(bWrite,'w',100);
		BOOL fGoOn=TRUE;
		PrintHelp();
		while(fGoOn)
		{
			char cGet = getchar();
			if( cGet ==0x0a || cGet== 0x0d)
				continue;
			int iRead,iRet,iWrote;
			switch (cGet)
			{
			case '1':
				s.Send(100,bWrite,iWrote);
				s.Recv(100,bRead,iRead);
				printf("send %d,recv %d\n",iWrote,iRead);
				break;
			case '2':
				s.Send(100,bWrite,iWrote);
				printf("send %d\n",iWrote);
				break;
			case '3':
				s.RecvWithTimeOut(5,100,bRead,iRead);
				printf("recv %d\n",iRead);
				break;
			case '4':
				s.TestAndRecv(5,100,bRead,iRead);
				printf("recv %d\n",iRead);
				break;
			case '0':
				fGoOn=FALSE;
				break;
			}
			PrintHelp();
		}
	}

	return nRetCode;
}


