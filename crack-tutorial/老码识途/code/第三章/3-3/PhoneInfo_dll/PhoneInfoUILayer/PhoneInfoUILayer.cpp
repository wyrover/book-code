// PhoneNote1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/types.h"

#include "../PhoneInfoLogicLayer/PhoneInfo.h"
#include "PhoneInfoUI.h"

void usage()
{
	printf("cmd as follows:\n");
	printf("load notebook: -ld path\n");
	printf("save notebook as file: -sa filename\n");
	printf("save notebook: -se \n");
	printf("add phone info: -ad\n");
	printf("quit program: -qt path\n");	
	printf("query name: -qy name\n");
}

char * getArg(char * start)
{
	int i = 0;
	if (start[0] == NULL) 
		return NULL;
	//strip space before
	while (start[i] == ' ')
	{
		i++;		
	}
	if (start[i] == NULL)
	{
		return NULL;
	}
	return &start[i];
}

BOOLEAN handleInput()
{
	char cmd[20];
	char* arg;
	char err[256];
	PhoneInfo info;
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
			return TRUE;
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
			return TRUE;
		}
		if (CopyFileA(gPhoneInfoPath, arg, FALSE) == 0)
		{				
			memset(err, 0, 256);
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, err, 256, NULL);
			printf("fail to copy: %s\n", err);
			return TRUE;
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
			return TRUE;
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
		return FALSE;
	}
	else
		usage();
	return TRUE;
}
