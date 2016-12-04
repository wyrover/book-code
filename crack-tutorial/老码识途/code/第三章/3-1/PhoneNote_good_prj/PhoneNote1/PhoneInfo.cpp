#include "PhoneInfo.h"
#include "utils.h"

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
		return;
	}
	//make the path
	strcpy(gPhoneInfoPath + i, DEFAULT_PHONE_INFO);
}


void addPhoneInfo(char * phoneInfo)
{
	FILE * pf;
	pf = fopen(gPhoneInfoPath, "a");
	if (pf == NULL)
	{
		return;
	}
	fwrite(phoneInfo, strlen(phoneInfo), 1, pf);
	fwrite((void *)"\r\n", 2, 1, pf);
	fclose(pf);
}
