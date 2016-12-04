// PhoneNote1.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "utils.h"
#include "../include/types.h"
#include "PhoneInfo.h"

void main(int argc, char* argv[])
{
	char cmd[20];
	char* arg;
	char err[256];
	PhoneInfo info;

	initInfo();
	getDefaultFilePath(argv[0]);
	if (hasDefaultPhoneInfo())
	{
		loadPhoneInfo(gPhoneInfoPath);		
	}	

	while(1)
	{
		printf("please input:");	
		scanf("%[^\n]", cmd);
		fflush(stdin);
		char oldC = cmd[3];
		cmd[3] = 0;
		if (strcmp(cmd, "-ld") == 0)
		{
			cmd[3] = oldC;
			arg = getArg(cmd + 3);
			if (arg == NULL)
			{
				printf("lose load file arg\n");
				usage();
				continue;
			}
			loadPhoneInfo(arg);
		}
		else if (strcmp(cmd, "-sa") == 0)
		{
			cmd[3] = oldC;
			arg = getArg(cmd + 3);
			if (arg == NULL)
			{
				printf("lose saved file arg\n");
				usage();
				continue;
			}
			if (CopyFileA(gPhoneInfoPath, arg, FALSE) == 0)
			{				
				memset(err, 0, 256);
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, err, 256, NULL);
				printf("fail to copy: %s\n", err);
				continue;
			}
		}
		else if (strcmp(cmd, "-qy") == 0)
		{
			cmd[3] = oldC;
			arg = getArg(cmd + 3);
			if (arg == NULL)
			{
				printf("lose queried name\n");
				usage();
				continue;
			}
			queryName(arg);
		}
		else if (strcmp(cmd, "-ad") == 0)
		{
			printf("name: ");
			scanf("%[^\n]", info.name);
			fflush(stdin);
			printf("phone: ");
			scanf("%[^\n]", info.phone);
			fflush(stdin);
			addPhoneInfo(info);
			printf("\n");
		}
		else if (strcmp(cmd, "-qt") == 0)
		{
			break;
		}
		else
			usage();
	}	
}

