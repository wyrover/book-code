#include "PhoneInfo.h"
#include "utils.h"

char gPhoneInfoPath[100];

PhoneInfo gPhoneInfoList[MAX_PHONE_NUM];

PhoneInfo * pCurrentInfo;

int gCount;

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

BOOLEAN canAdd()
{
	return gCount < MAX_PHONE_NUM ? TRUE : FALSE;
}

void loadPhoneInfo(char * file)
{
	FILE * fp;
	char line[128];	
	int i;

	fp = fopen(file, "rb");
	if (fp == NULL)
	{
		printf("cannot find file %s\n", file);
		return;
	}
	
	initInfo();	
	while (feof(fp) == 0)
	{
		if (canAdd() == FALSE)
		{
			printf("the count of phone info is over than %d , cannot load all\n", MAX_PHONE_NUM);
			break;
		}
		if (fread(pCurrentInfo, sizeof(PhoneInfo), 1, fp) == 0)
			break;
		printf("%s   %s\n", pCurrentInfo->name, pCurrentInfo->phone);
		pCurrentInfo++;
		gCount++;
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


void addPhoneInfo(PhoneInfo info)
{
	FILE * pf;

	if (canAdd() == FALSE)
	{
		printf("cannot add any more\n");
		return;
	}
	pf = fopen(gPhoneInfoPath, "ab");
	if (pf == NULL)
	{
		return;
	}
	fwrite(&info, sizeof(PhoneInfo), 1, pf);	
	* pCurrentInfo = info;
	pCurrentInfo++;
	gCount++;
	fclose(pf);
}

void initInfo()
{
	pCurrentInfo = gPhoneInfoList;
	gCount = 0;
}

void queryName(char * name)
{
	int i;
	BOOLEAN found = FALSE;
	for (i = 0; i < gCount; i++)
	{
		if (strcmp(gPhoneInfoList[i].name, name) == 0)
		{
			printf("%s  %s\n", gPhoneInfoList[i].name, gPhoneInfoList[i].phone);
			found = TRUE;
		}
	}

	if (found == FALSE)
	{
		printf("cannot find the name: %s\n", name);
	}
}