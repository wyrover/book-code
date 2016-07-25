/*程序以命令的方式运行
  命令格式如下：
  server [-p:x] [-i:IP] [-o]
  -p:x     进行监听的端口号
  -i:str   进行监听的IP,应用于多ip服务器
  -o       只接收消息，不发送回应消息
*/
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PORT 5150 /*默认端口号*/
#define DEFAULT_BUFFER 4896 /*缓冲区大小*/

unsigned short iPort = DEFAULT_PORT;
BOOL bInterface = FALSE, bRecvOnly = FALSE;
char szAddress[128];

/*显示参数列表*/
void usage()
{
	printf("usage: server [-p:x] [-i:IP] [-o]\n\n");
	printf("       -p:x      Port number to listen on\n");
	printf("       -i:str    Interface to listen on\n");
	printf("       -o        Don't echo the data back\n\n");
	ExitProcess(1);
}

/*验证参数*/
void ValidateArgs(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
	{
		if ((argv[i][0] =='-') || (argv[i][0] == '/'))
		{
			switch (tolower(argv[i][1]))
			{
			/*指定端口*/
			case 'p':
				iPort = atoi(&argv[i][3]);
				break;

			/*指定IP*/
			case 'i':
				bInterface = TRUE;
				if (strlen(argv[i]) > 3)
					strcpy(szAddress, &argv[i][3]);
				break;

			/*只接收消息，不回应*/
			case 'o':
				bRecvOnly = TRUE;
				break;

			/*非法参数，显示参数列表*/
			default:
				usage();
				break;
			}
		}
	}
}

/*与客户机通信的线程函数*/
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET sock = (SOCKET)lpParam;
	char szBuff[DEFAULT_BUFFER];
	int ret, nLeft, idx;
	while (1)
	{
		/*接收来自客户机的消息*/
		ret = recv(sock, szBuff,DEFAULT_BUFFER, 0);
		if (ret == 0)
			break;
		else if (ret == SOCKET_ERROR)
		{
			printf("recv() failed: %d\n", WSAGetLastError());
			break;
		}
		szBuff[ret] = '\0';
		printf("RECV: %s\n", szBuff);

		if (!bRecvOnly)
		{
			nLeft = ret;
			idx = 0;
			while (nLeft > 0)
			{
				/*向客户机发送回应消息*/
				ret = send(sock, &szBuff[idx], nLeft, 0);
				if (ret == 0)
					break;
				else if (ret == SOCKET_ERROR)
				{
					printf("send() failed: %d\n", WSAGetLastError());
					break;
				}
				nLeft -=ret;
				idx +=ret;
			}
		}

	}
	return 0;
}

int main(int argc, char **argv)
{
	WSADATA wsd;
	SOCKET sListen, sClient;
	int iAddrSize;
	HANDLE hThread;
	DWORD dwThread;
	struct sockaddr_in local, client;
	
	/*验证参数*/
	ValidateArgs(argc, argv);
	
	/*加载Winsock DLL*/
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("Failed to load WinSock!\n");
		return 1;
	}

	/*创建Socket*/
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sListen == SOCKET_ERROR)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		return 1;
	}
	if (bInterface)
	{
		local.sin_addr.s_addr = inet_addr(szAddress);
		if (local.sin_addr.s_addr == INADDR_NONE)
		{
			usage();
		}
	}
	else 
	{
		local.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	local.sin_family = AF_INET;
	local.sin_port = htons(iPort);

	/*绑定Socket*/
	if (bind(sListen, 
		     (struct sockaddr*)&local,
			 sizeof(local)) == SOCKET_ERROR)
	{
		printf("bind() failed: %d\n", WSAGetLastError());
		return 1;
	}
	/*打开监听*/
	listen(sListen, 8);

	/*在端口进行监听，一旦有客户机发起连接请示
	就建立与客户机进行通信的线程*/
	while(1)
	{
		iAddrSize = sizeof(client);
		/*监听是否有连接请求*/
		sClient = accept(sListen, 
						 (struct sockaddr*)&client, 
						 &iAddrSize);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept() failed: %d\n",WSAGetLastError());
			break;
		}
		printf("Acceped client: %s:%d\n",
			   inet_ntoa(client.sin_addr), 
			   ntohs(client.sin_port));

		hThread = CreateThread(NULL, 
							   0, 
							   ClientThread,
					           (LPVOID)sClient, 
							   0, 
							   &dwThread);
		if (hThread == NULL)
		{
			printf("CreateThread() failed: %d\n", GetLastError());
			break;
		}
		CloseHandle(hThread);
	}
	closesocket(sListen);
	WSACleanup();
	return 0;
}