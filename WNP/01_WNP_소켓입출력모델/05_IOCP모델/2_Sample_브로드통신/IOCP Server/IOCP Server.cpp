#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>

#define PORT 25000
#define DATA_BUFSIZE 1024

struct DATA
{
	int type;
	char str[100];
	int num1;
	int num2;
};

typedef struct
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	char Buffer[DATA_BUFSIZE];
	DATA data;
	DWORD BytesSEND;
	DWORD BytesRECV;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct
{
	SOCKET Socket;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

PER_HANDLE_DATA **HandleArray;
PER_IO_OPERATION_DATA **DataArray;
int Index = 0;

unsigned _stdcall ServerWorkerThread(LPVOID CompletionPortID);
void main ( )
{
	SOCKADDR_IN InternetAddr;
	SOCKET Listen;
	SOCKET Accept;
	HANDLE CompletionPort;
	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	int i;
	DWORD RecvBytes;
	DWORD Flags;
	unsigned ThreadID;
	WSADATA wsaData;
	DWORD Ret;

	if((Ret = WSAStartup(0x202, &wsaData)) != 0)
	{
		printf("WSASartup error");
		return;
	}
	if((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) ==NULL)
	{
		return;
	}

	GetSystemInfo(&SystemInfo);
	for( i = 0 ; i < (int) SystemInfo.dwNumberOfProcessors*2; i++)
	{
		HANDLE ThreadHandle;
		if((ThreadHandle = (HANDLE) _beginthreadex(NULL,
			0, &ServerWorkerThread, CompletionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() error");
			return;
		}
		CloseHandle(ThreadHandle);
	}
	if((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL,
		0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket( ) error");
		return;
	}
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(PORT);
	if(bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		printf("bind() error");
		return;
	}
	if(listen(Listen,  5 ) == SOCKET_ERROR)
	{
		printf("listen( ) error");
	}

	HandleArray = (PER_HANDLE_DATA**)malloc(sizeof(PER_HANDLE_DATA) * 10);
	DataArray = (PER_IO_OPERATION_DATA**)malloc(sizeof(PER_IO_OPERATION_DATA) * 10);
	
	while(TRUE)
	{
		if((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) ==
			SOCKET_ERROR)
		{
			printf("WSAAccept() error");
			return;
		}
		if((PerHandleData = 
			(LPPER_HANDLE_DATA) malloc(sizeof(PER_HANDLE_DATA))) == NULL)
		{
			printf("malloc( ) error");
			return;
		}
		printf("Socket number %d connected\n", Accept);
		PerHandleData->Socket = Accept;
		HandleArray[Index] = PerHandleData;
		
		if(CreateIoCompletionPort((HANDLE) Accept,	CompletionPort,
			(DWORD) PerHandleData, 0 ) == NULL)
		{
			printf("CreateIoCompletionPort error");
			return;
		}

		if((PerIoData = (LPPER_IO_OPERATION_DATA) malloc(sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("malloc( ) error");
			return;
		}
		DataArray[Index] = PerIoData;
		Index++;
				
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;

		Flags = 0;
		if(WSARecv(Accept, &(PerIoData->DataBuf), 1, &RecvBytes,
			&Flags, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		{
			if(WSAGetLastError( ) != ERROR_IO_PENDING)
			{
				printf("WSARecv( ) error");
				return;
			}
		}
		else
		{
			memcpy(&PerIoData->data, (DATA*)PerIoData->Buffer, sizeof(DATA));

			if(PerIoData->data.type == 0)
				printf("%s\n", PerIoData->data.str);
			else if(PerIoData->data.type == 1)
				printf("%d\n", PerIoData->data.num1 + PerIoData->data.num2);
		}
	}
}

unsigned __stdcall ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE CompletionPort = (HANDLE) CompletionPortID;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;
	int i;

	while(TRUE)
	{
		if(GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
			(LPDWORD)&PerHandleData, (LPOVERLAPPED*) &PerIoData, INFINITE) == 0)
		{
			printf("GetQueuedCompletionStatus error");
			return 0;
		}
		if(BytesTransferred == 0 )
		{
			printf("Closing Socket %d \n", PerHandleData->Socket);
			if(closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("closesocket( ) error");
				return 0;
			}
			free(PerHandleData);
			free(PerIoData);
			continue;
		}
		if(PerIoData->BytesRECV == 0)
		{
			PerIoData->BytesRECV = BytesTransferred;
			PerIoData->BytesSEND = 0;
		}
		else
		{
			PerIoData->BytesSEND +=BytesTransferred;
		}
		if(PerIoData->BytesRECV > PerIoData->BytesSEND)
		{
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->DataBuf.buf = PerIoData->Buffer + PerIoData->BytesSEND;
			PerIoData->DataBuf.len = PerIoData->BytesRECV - PerIoData->BytesSEND;

			for(i=0; i<Index; i++)
			{
				PerHandleData = HandleArray[i];
				//PerIoData = DataArray[i];
				
				if(WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes,
					0, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
				{
					if(WSAGetLastError( ) != ERROR_IO_PENDING)
					{
						printf("WSASend( ) error");
						return 0;
					}
				}
				
			}
						
		}
		else
		{
			PerIoData->BytesRECV = 0;
			Flags = 0 ;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			memcpy(&PerIoData->data, (DATA*)PerIoData->Buffer, sizeof(DATA));

			if(PerIoData->data.type == 0)
				printf("%s\n", PerIoData->data.str);
			else if(PerIoData->data.type == 1)
				printf("%d\n", PerIoData->data.num1 + PerIoData->data.num2);
			printf("Recv2\n");

			if(WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1,
				&RecvBytes, &Flags, &(PerIoData->Overlapped), NULL)
				 == SOCKET_ERROR)
			{
				if(WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv() Error");
					return 0;
				}
				
			}
		
		}
	}
	_endthreadex(0);
}


