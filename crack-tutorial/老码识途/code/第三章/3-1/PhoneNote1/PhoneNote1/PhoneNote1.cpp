// PhoneNote1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define DEFAULT_PHONE_INFO "phoneinfo.txt"
#define TRUE 1
#define FALSE 0
#define BOOLEAN int
#define END_OF_FILE -1

char gPhoneInfoPath[100];

void usage()
{
	printf("cmd as follows:\n");
	printf("load notebook: -ld path\n");
	printf("save notebook as file: -sa filename\n");
	printf("save notebook: -se \n");
	printf("add phone info: -ad name phone\n");
	printf("quit program: -qt path\n");	
}

BOOLEAN hasDefaultPhoneInfo()
{
	FILE * p = fopen(gPhoneInfoPath, "r");
	if (p != NULL)
	{
		fclose(p);
		return TRUE;
	}
	return FALSE;
}

int readLn(FILE * fp, char * ln)
{
	int c, ret;
	ret = 0;
	
	while (1)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			*ln = 0;
			return ret;
		}
		*ln = (char)c;
		ln++;
		ret++;
		if (c == '\n')
		{
			if (*(ln - 2) == '\r')
			{
				*(ln - 2) = 0;
				ret--;
			}
			else
				*(ln - 1) = 0;
			return ret;
		}
	}
	return ret;
}

void loadPhoneInfo(char * file)
{
	FILE * fp;
	char line[128];

	fp = fopen(file, "r");
	if (fp == NULL)
	{
		printf("cannot find file %s\n", file);
		return;
	}

	while (feof(fp) == 0)
	{
		readLn(fp, line);
		printf("%s\n", line);
	}
	fclose(fp);
}

void getDefaultFilePath(char* exePath)
{
	//find directory
	int i;
	i = strlen(exePath) - 1;
	strcpy(gPhoneInfoPath, exePath);
	while (i >= 0)
	{
		if (gPhoneInfoPath[i] == '\\')
		{
			i++;
			break;
		}	
		i--;
	}
	if (i < 0)
	{
		strcpy(gPhoneInfoPath, DEFAULT_PHONE_INFO);
		return;
	}
	//make the path
	strcpy(gPhoneInfoPath + i, DEFAULT_PHONE_INFO);	
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

void addPhoneInfo(char * phoneInfo)
{
	FILE * pf;
	pf = fopen(gPhoneInfoPath, "ab");
	if (pf == NULL)
	{
		return;
	}
	fwrite(phoneInfo, strlen(phoneInfo), 1, pf);
	fwrite((void *)"\r\n", 2, 1, pf);
	fclose(pf);
}

void main(int argc, char* argv[])
{
	char cmd[20];
	char* arg;
	char err[256];
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
		else if (strcmp(cmd, "-ad") == 0)
		{
			cmd[3] = oldC;
			arg = getArg(cmd + 3);
			if (arg == NULL)
			{
				printf("lose added info arg\n");
				usage();
				continue;
			}
			addPhoneInfo(arg);
		}
		else if (strcmp(cmd, "-qt") == 0)
		{
			break;
		}
		else
			usage();
	}	
}

