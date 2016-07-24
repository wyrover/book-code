// pipeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pipeServer.h"
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
		while(1)
		{
			HANDLE hP = SPCreateServerPipe("\\\\.\\pipe\\tp",100);
			if ( hP == INVALID_HANDLE_VALUE)
				continue;

			else
			{
				printf("[%d] connected ...\n",hP);

				CIPCNamedPipe s(hP,TRUE);
				BOOL fOK = TRUE;
				printf("perpare to reading\n");
				while(fOK)
				{
					int iRead;
					BYTE bBuf[100];
					//int iTest = s.TestRead(1);
					
					//60秒内读入100字节
					int iRet = s.RecvWithTimeOut(60,100,bBuf,iRead);
					if( iRet == SP_ERR_SUCCESS)
					{
						for(int i=0;i<10;i++)
						{//在10秒钟内返回数据
							printf("[%d] send once %d\n",hP,i+1);
							int iWrote=0;
							iRet = s.Send(10,bBuf+i*10,iWrote);
							if(iRet !=SP_ERR_SUCCESS)
							{
								printf("\tsend error\n");
								fOK = FALSE;
								break;
							}
							else
								printf("\tsent %d\n",iWrote);
							Sleep(1000);
						}
					}
					else
					{
						printf("recv error %d\n",iRet);
						fOK = FALSE;
					}
				}
				printf("[%d] disconnected ...\n",hP);
			}
		}
	}

	return nRetCode;
}


