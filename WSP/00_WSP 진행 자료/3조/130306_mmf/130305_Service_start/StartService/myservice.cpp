//myservice.cpp
#include "std.h"
//HANDLE hRead, hWrite;
HANDLE hEvent, hMap;
PACKET *pData;

void myservice_init()
{
	//mmf_init();
	DWORD ThreadID;
	CloseHandle(CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID));
	
}

void myservice_run()
{
	
}

void myservice_exit()
{
	UnmapViewOfFile( pData );
	CloseHandle( hMap );
	CloseHandle( hEvent );
}

void myservice_newfile()
{
	
}

DWORD WINAPI ThreadFunc(LPVOID temp)
{
	sever_init();
	sock_init(myrefun);
	mmf_init();
	server_run();
	sever_exit();
	return 0;
}

void myrefun(char*i,int size,SOCKET sock)
{

/*	HANDLE hPipe = CreateFile( "\\\\.\\pipe\\source_server", GENERIC_READ, 
									FILE_SHARE_WRITE | FILE_SHARE_READ,
									0, OPEN_EXISTING, 
									FILE_ATTRIBUTE_NORMAL, 0);
	
	if ( hPipe == INVALID_HANDLE_VALUE )
	{		
		MessageBox( 0, "실패", "", MB_OK);
		return;
	}
	
	DWORD len;
	WriteFile( hPipe, i,strlen(i), &len, 0);
	
#ifndef MY_DEBUG
	PACKET *p = (PACKET*)i;
	printf("[%d]--%s--%s--%s\r\n",p->flag,p->s_message,p->s_name,p->s_time );
#endif
	FlushFileBuffers( hPipe );
	CloseHandle( hPipe );
*/
	//mmf_init();

#ifndef MY_DEBUG
	printf("3\n");
#endif

	PACKET *p = (PACKET*)i;
	pData->flag = p->flag ;
	strcpy(pData->s_message,p->s_message);  
	strcpy(pData->s_name,p->s_name);
	strcpy(pData->s_time,p->s_time);

#ifndef MY_DEBUG
	printf("[%d]--%s--%s--%s\r\n",pData->flag,pData->s_message,pData->s_name,pData->s_time );
#endif
	
	SetEvent( hEvent );
#ifndef MY_DEBUG
	printf("4");
#endif
		
}

void mmf_init()
{
#ifndef MY_DEBUG
	printf("1\n");
#endif
	
	SECURITY_DESCRIPTOR    sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = &sd;



	hEvent = CreateEvent( &sa, 0, 0, "MYMESSAGE");

	hMap = CreateFileMapping( (HANDLE)-1, // Paging 화일을 사용해서 매핑
								  &sa, PAGE_READWRITE, 0, sizeof(PACKET), "mymmf");

	
	if ( hMap == 0 )
	{
		MessageBox( 0, "Fail", "", MB_OK);		
		return ;
	}
#ifndef MY_DEBUG
	printf("%d\n",hMap);
	printf("%d\r\n",hEvent);
#endif
	pData = (PACKET*)MapViewOfFile( hMap, FILE_MAP_WRITE, 0, 0,0);

	if ( pData == 0 )
	{
		MessageBox( 0, "Fail", "", MB_OK);		
		return ;
	}

#ifndef MY_DEBUG
	printf("2");
#endif

}

