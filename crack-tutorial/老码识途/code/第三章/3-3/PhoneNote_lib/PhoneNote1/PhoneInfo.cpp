#include "PhoneInfo.h"
#include "utils.h"
#include "../include/simplediskarray.h"

char gPhoneInfoPath[100];

DynamicArray gPhoneInfoList;

void clearInfo();

int getInfoCount()
{
	return gPhoneInfoList.count;
}

PhoneInfo getInfo(int index)
{
	return *(PhoneInfo *)getArrayItem(index, gPhoneInfoList);	
}

void showPhoneInfo(PhoneInfo * info)
{
	printf("%s   %s\n", info->name, info->phone);
}

ShowPhoneInfoPtr pShowPhoneInfo = showPhoneInfo;

void setShowPhoneInfo(ShowPhoneInfoPtr pfunc)
{
	pShowPhoneInfo = pfunc;
}

void addInfo(PhoneInfo * info)
{
	setArrayLength(&gPhoneInfoList, gPhoneInfoList.count + 1);
	setArrayItem(gPhoneInfoList.count - 1, gPhoneInfoList, info);	
}

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
	return TRUE;
}

void loadPhoneInfo(char * file)
{
	clearInfo();		
	SimpleDiskArray diskArray;
	initSimpleDiskArray(&diskArray, sizeof(PhoneInfo), file);
	if (openSimpleDiskArray(&diskArray) == FALSE)
	{
		return;
	}
	int count = getSimpleDiskArrayCount(&diskArray);
	PhoneInfo info;
	for (int i = 0; i < count; i++)
	{
		if (getSimpleDiskArray(&diskArray, i, &info) == FALSE)
			return;
		pShowPhoneInfo(&info);
		addInfo(&info);
	}
	closeSimpleDiskArray(&diskArray);
	finalizeSimpleDiskArray(&diskArray);
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
	if (canAdd() == FALSE)
	{
		printf("cannot add any more\n");
		return;
	}
	SimpleDiskArray diskArray;
	initSimpleDiskArray(&diskArray, sizeof(PhoneInfo), gPhoneInfoPath);
	if (openSimpleDiskArray(&diskArray) == FALSE)
	{
		return;
	}
	insertSimpleDiskArray(&diskArray, &info);
	addInfo(&info);
	closeSimpleDiskArray(&diskArray);
	finalizeSimpleDiskArray(&diskArray);
}

void initInfo()
{	
	initDynamicArray(&gPhoneInfoList, sizeof(PhoneInfo));	
}

void clearInfo()
{
	setArrayLength(&gPhoneInfoList, 0);
}

BOOLEAN queryName(char * name)
{
	int i;
	int count = getInfoCount();
	for (i = 0; i < count; i++)
	{
		PhoneInfo info = getInfo(i);
		if (strcmp(info.name, name) == 0)
		{
			pShowPhoneInfo(&info);
			return TRUE;
		}
	}
	printf("cannot find the name: %s\n", name);
	return FALSE;
}