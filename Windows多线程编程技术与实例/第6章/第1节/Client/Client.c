/*程序以命令的方式运行
  命令格式如下：
  client [-p:x] [-s:IP] [-n:x] [-o]
  -p:x     远程服务器端口号
  -s:IP    远程服务器IP
  -n:x     消息发送的次数
  -o       只发送消息，不接收回应消息
*/
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_COUNT 20 /*消息发送的默认次数*/ 
#define DEFAULT_PORT 5150 /*默认端口号*/
#define DEFAULT_BUFFER 2048 /*缓冲区大小*/
#define DEFAULT_MESSAGE "This is a socket sample" /*发送的消息内容*/

char szServer[128], szMessage[1024];
unsigned short iPort = DEFAULT_PORT;
DWORD dwCount = DEFAULT_COUNT;
BOOL bSendOnly = FALSE;

/*显示参数列表*/
void usage()
{
	printf("usage: client [-p:x] [-s:IP] [-n:x] [-o]\n\n");
	printf("       -p:x     Remote port to send to \n");
	printf("       -s:IP    Server's IP address or host name\n");
	printf("       -n:x     Number of times to send message\n");
	printf("       -o       Send messages only: don't receive\n");
	ExitProcess(1);
}

/*验证参数*/
void ValidateArgs(int argc, char *argv[])
{
	int i;
	for (i = 0; i < argc; i++)
	{
		if ((argv[i][0] == '-') || (argv[i][0] == '/'))
		{
			switch (tolower(argv[i][1]))
			{
				/*设置与之通信的服务器端口*/
				case 'p':
					if (strlen(argv[i]) > 3)
						iPort = atoi (&argv[i][3]);
					break;

				/*设置与之通信的服务器IP*/
				case 's':
					if (strlen(argv[i]) > 3)
						strcpy(szServer, &argv[i][3]);
					break;

				/*设置消息的发送次数*/
				case 'n':
					if (strlen(argv[i]) > 3)
						dwCount = atol(&argv[i][3]);
					break;

				/*只发送消息，不接收回应消息*/
				case 'o':
					bSendOnly = TRUE;
					break;
				/*参数非法，显示参数列表*/	
				default:
					usage();
					break;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	WSADATA wsd;
	SOCKET sClient;
	char szBuffer[DEFAULT_BUFFER];
	int ret;
	struct sockaddr_in server;
	struct hostent *host = NULL;
	DWORD i;
	
	/*验证参数*/
	ValidateArgs(argc, argv);
	/*加载Winsock DLL*/
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("Failed to load Winsock library!\n");
		return 1;
	}
	strcpy(szMessage, DEFAULT_MESSAGE);
	
	/*创建Socket*/
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		return 1;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(szServer);

	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			printf("Unable to resolve server: %s\n", szServer);
			return 1;
		}
		CopyMemory(&server.sin_addr, 
				   host->h_addr_list[0],
			       host->h_length);
	}
	/*与服务器建立连接*/
	if (connect(sClient, 
		        (struct sockaddr*)&server,
			    sizeof(server)) 
		== SOCKET_ERROR)
	{
		printf("connect() failed: %d\n", WSAGetLastError());
		return 1;
	}

	/*发送、接收消息*/
	for (i = 0; i < dwCount; i++)
	{
		ret = send(sClient, szMessage, strlen(szMessage), 0);
		if (ret ==0 )
		{
			break;
		}
		else if (ret == SOCKET_ERROR)
		{
			printf("send() failed: %d\n", WSAGetLastError());
			break;
		}
		printf("Send %d bytes\n", ret);
		if (!bSendOnly)
		{
			ret = recv(sClient, szBuffer, DEFAULT_BUFFER, 0);
			if (ret == 0)
			{
				break;
			}
			else if (ret == SOCKET_ERROR)
			{
				printf("recv() failed: %d\n", WSAGetLastError());
				break;
			}
			szBuffer[ret] = '\0';
			printf("RECV [%d bytes]: %s\n", ret, szBuffer);
		}
	}
	closesocket(sClient);
	WSACleanup();
	return 0;
}